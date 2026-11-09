#!/usr/bin/env python3
"""Generate seed files for fuzz/unpack_pack_fuzzer_seed_corpus.

The committed raw seed files in unpack_pack_fuzzer_seed_corpus/ were produced
by this script; re-run it from this directory to regenerate them. It emits a
canonical MessagePack byte sample for every format family (ints at their width
edges, floats, str/bin size encodings, arrays/maps at their 16/32 boundaries,
ext types, timestamps, deep nesting, concatenated messages) plus a few
truncated/malformed samples so the error paths are seeded too. The incumbent
seed corpus ships only five files (EmptyArray, EmptyObject, ExcessiveNesting,
OpenWeatherMap, WeatherUnderground); this widens type coverage without
duplicating those names.
"""
import os

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, 'unpack_pack_fuzzer_seed_corpus')

SEEDS = {
    'GenFixintEdges': b'\x00\x7f\xe0\xff',
    'GenNilBool': b'\xc0\xc2\xc3',
    'GenInt8Edges': b'\xd0\x80\xd0\x7f\xd0\xff\xd0\x00',
    'GenInt16Edges': b'\xd1\x80\x00\xd1\x7f\xff\xd1\xff\xff',
    'GenInt32Edges': b'\xd2\x80\x00\x00\x00\xd2\x7f\xff\xff\xff\xd2\xff\xff\xff\xff',
    'GenInt64Edges': b'\xd3\x80' + b'\x00'*6 + b'\xd3\x7f' + b'\xff'*6 + b'\xd3\xff'*8,
    'GenUintEdges': b'\xcc\xff\xcd\xff\xff\xce\xff\xff\xff\xff\xcf' + b'\xff'*8,
    'GenFloats': b'\xca\x3f\x80\x00\x00\xca\x7f\x7f\xff\xff\xcb\x40\x09\x21\xfb\x54\x44\x2d\xcb\x00\x00\x00\x00\x00\x00\x00\x01',
    'GenStrFix': b'\xa1x\xa3abc',
    'GenStrUtf8': b'\xa6' + 'caf\u00e9'.encode('utf-8') + b'\xa4' + '\U0001f600'.encode('utf-8'),
    'GenStr255': b'\xd9\xff' + b'a'*255,
    'GenStr256': b'\xda\x01\x00' + b'b'*256,
    'GenBinSizes': b'\xc4\x05' + b'\x01'*5 + b'\xc5\x01\x00' + b'\x02'*256 + b'\xc4\x00',
    'GenArrays': b'\x90\x92\x01\x02\x93\x01\x92\x02\x03\x04',
    'GenNested32': b'\x91'*32 + b'\x01',
    'GenMaps': b'\x80\x81\xa1a\x01\x82\xa1k\xa1v\xa1j\xa2vv',
    'GenExtTypes': b'\xd4\x05\x01\xd5\x06\x02\x03\xd6\x07' + b'\x04'*4 + b'\xd8\x08' + b'\x09'*16,
    'GenTimestamps': b'\xd6\xff\x00\x00\x00\x00\xd7\xff' + (b'\x01'*8) + b'\xc7\x0c\xff' + (b'\x00'*4) + (b'\x02'*8),
    'GenMultiMessage': b'\x01\xc2\x91\x02\xa1z',
    'GenTruncatedStr': b'\xd9\x05ab',
    'GenMalformedMap': b'\xde\x00\x02\xa1a\x01',
    'GenArray32Short': b'\xdd\x00\x00\x00\x01\x01',
    'GenMapStrInt': b'\x83\xa1x\x01\xa1y\x02\xa1z\x03',
    'GenMixedTree': (b'\x93\x81\xa1k\xc7\x03\x07\x01\x02\x03\x92\xd0\xfe\xcc\x2a\xcb\xbf\xf0\x00\x00\x00\x00\x00\x10\xc0'),
}

def main():
    os.makedirs(OUT, exist_ok=True)
    for name, data in SEEDS.items():
        with open(os.path.join(OUT, name), 'wb') as f:
            f.write(data)
    print('wrote %d seeds to %s' % (len(SEEDS), OUT))

if __name__ == '__main__':
    main()
