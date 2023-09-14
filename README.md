# JPEG Encoder
![build](https://github.com/JakubOchnik/jpeg-encoder/actions/workflows/cmake.yml/badge.svg)  
This is a simple implementation of JPEG encoder. It will be merged into the [CUDAimageProcessor](https://github.com/JakubOchnik/CUDAimageProcessor) in the future. Work in progress! 

## How does it work?
Ultimately, the encoder will follow a basic JPEG pipeline:
1. Import
2. RGB2YCbCr conversion
3. Chroma subsampling
4. DCT + quantization + zig-zag scan
5. Hufmann encoding
6. Delta encoding
7. Header
8. Final export
___
## Currently working:
1. Conversion between RGB and YCbCr color spaces
2. 4:1:1 chroma subsampling and desampling
3. DCT
4. Forward Zig-zag scan

## TODO:
#### Encoding
- Huffmann + delta encoding
- Header handling (write)
- Exporting actual image
#### Decoding
- Importing actual image
- Header handling (read)
- Inverse Huffmann + delta encoding
- Inverse DCT
#### Miscellaneous
- Faster DCT algorithm
- 4:2:2 and 4:2:0 subsampling modes
- Parallel implementation
