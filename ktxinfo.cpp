/**
  @file ktxinfo.cpp

  ktxinfo print ktx file information.
*/
#include "ktx.h"
#include <iostream>
#include <string>
#include <stdint.h>
#include <stdio.h>

void print_usage()
{
  std::cout << "ktxinfo ver 1.0" << std::endl;
  std::cout << "Print KTX header information." << std::endl;
  std::cout << std::endl;
  std::cout << "usage: ktxinfo.exe [-h] [filename]" << std::endl;
  std::cout << std::endl;
  std::cout << "  -h        Print this text." << std::endl;
  std::cout << "  filename  A file that has KTX header." << std::endl;
}

std::string get_gl_name(uint32_t n)
{
  switch (n) {
  case 0x0000: return "(0)";

  // from gl2.h
  case 0x1400: return "GL_BYTE";
  case 0x1401: return "GL_UNSIGNED_BYTE";
  case 0x1402: return "GL_SHORT";
  case 0x1403: return "GL_UNSIGNED_SHORT";
  case 0x1404: return "GL_INT";
  case 0x1405: return "GL_UNSIGNED_INT";
  case 0x1406: return "GL_FLOAT";
  case 0x140C: return "GL_FIXED";
  case 0x8033: return "GL_UNSIGNED_SHORT_4_4_4_4";
  case 0x8034: return "GL_UNSIGNED_SHORT_5_5_5_1";
  case 0x8363: return "GL_UNSIGNED_SHORT_5_6_5";
  case 0x1902: return "GL_DEPTH_COMPONENT";
  case 0x1906: return "GL_ALPHA";
  case 0x1907: return "GL_RGB";
  case 0x1908: return "GL_RGBA";
  case 0x1909: return "GL_LUMINANCE";
  case 0x190A: return "GL_LUMINANCE_ALPHA";
  case 0x8056: return "GL_RGBA4";
  case 0x8057: return "GL_RGB5_A1";
  case 0x8D62: return "GL_RGB565";
  case 0x81A5: return "GL_DEPTH_COMPONENT16";
  case 0x1901: return "GL_STENCIL_INDEX";
  case 0x8D48: return "GL_STENCIL_INDEX8";

  // from gl2ext.h
  case 0x8D64: return "GL_ETC1_RGB8_OES";
  case 0x8B90: return "GL_PALETTE4_RGB8_OES";
  case 0x8B91: return "GL_PALETTE4_RGBA8_OES";
  case 0x8B92: return "GL_PALETTE4_R5_G6_B5_OES";
  case 0x8B93: return "GL_PALETTE4_RGBA4_OES";
  case 0x8B94: return "GL_PALETTE4_RGB5_A1_OES";
  case 0x8B95: return "GL_PALETTE8_RGB8_OES";
  case 0x8B96: return "GL_PALETTE8_RGBA8_OES";
  case 0x8B97: return "GL_PALETTE8_R5_G6_B5_OES";
  case 0x8B98: return "GL_PALETTE8_RGBA4_OES";
  case 0x8B99: return "GL_PALETTE8_RGB5_A1_OES";
  case 0x8051: return "GL_RGB8_OES";                                            
  case 0x8058: return "GL_RGBA8_OES";
  case 0x8D61: return "GL_HALF_FLOAT_OES";
  case 0x8DF6: return "GL_UNSIGNED_INT_10_10_10_2_OES";
  case 0x8DF7: return "GL_INT_10_10_10_2_OES";
  case 0x87F9: return "GL_3DC_X_AMD";                                           
  case 0x87FA: return "GL_3DC_XY_AMD";
  case 0x8C92: return "GL_ATC_RGB_AMD";
  case 0x8C93: return "GL_ATC_RGBA_EXPLICIT_ALPHA_AMD";
  case 0x87EE: return "GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD";
  case 0x80E1: return "GL_BGRA_EXT";
  case 0x8365: return "GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT";
  case 0x8366: return "GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT";
  case 0x8C00: return "GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG";
  case 0x8C01: return "GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG";
  case 0x8C02: return "GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG";
  case 0x8C03: return "GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG";
  case 0x8368: return "GL_UNSIGNED_INT_2_10_10_10_REV_EXT";
  case 0x83F0: return "GL_COMPRESSED_RGB_S3TC_DXT1_EXT";
  case 0x83F1: return "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT";

  // OpenGL ES 2.0 accepts following types, but thye doesn't defines in gl2.h and gl2ext.h
  case 0x803C: return "GL_ALPHA8_EXT";
  case 0x8040: return "GL_LUMINANCE8_EXT";
  case 0x8045: return "GL_LUMINANCE8_ALPHA8_EXT";

  default: {
    char buf[32] = { 0 };
    snprintf(buf, sizeof(buf) - 1, "0x%04x", n);
    return buf;
  }
  }
}

int main(int argc, char** argv)
{
  std::string infilename;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && (argv[i][1] == 'h' || argv[i][1] == 'H')) {
      print_usage();
      return 0;
    }
    infilename = argv[i];
    break;
  }
  if (infilename.empty()) {
    print_usage();
    return 0;
  }

  KTX::File ktx;
  if (!KTX::read_texture(infilename, ktx)) {
    return -1;
  }
  const KTX::Endian endianness = KTX::get_endian(ktx.header);
  std::cout << "Endianness           : " << (endianness == KTX::Endian_Little ? "LITTLE" : "BIG") << std::endl;
  std::cout << "glType               : " << (ktx.header.glType != 0 ? get_gl_name(ktx.header.glType) : "(Compressed)") << std::endl;
  std::cout << "glTypeSize           : " << ktx.header.glTypeSize << std::endl;
  std::cout << "glFormat             : " << (ktx.header.glFormat != 0 ? get_gl_name(ktx.header.glFormat) : "(Compressed)") << std::endl;
  std::cout << "glInternalFormat     : " << get_gl_name(ktx.header.glInternalFormat) << std::endl;
  std::cout << "glBaseInternalFormat : " << get_gl_name(ktx.header.glBaseInternalFormat) << std::endl;
  std::cout << "pixelWidth           : " << ktx.header.pixelWidth << std::endl;
  std::cout << "pixelHeight          : " << ktx.header.pixelHeight << std::endl;
  std::cout << "pixelDepth           : " << ktx.header.pixelDepth << std::endl;
  std::cout << "numberOfArrayElements: " << ktx.header.numberOfArrayElements << std::endl;
  std::cout << "numberOfFaces        : " << ktx.header.numberOfFaces << std::endl;
  std::cout << "numberOfMipmapLevels : " << ktx.header.numberOfMipmapLevels << std::endl;
  std::cout << "bytesOfKeyValueData  : " << ktx.header.bytesOfKeyValueData << std::endl;
}
