import os

from bdflib import reader
from natsort import natsorted

# Codepoint range must contain 0x7F (fallback codepoint in the renderer)
CODEPOINT_START = 0x20  # Inclusive
CODEPOINT_END = 0xFF  # Inclusive
CODEPOINTS = [i for i in range(CODEPOINT_START, CODEPOINT_END + 1)]
ENCODING = "ISO-8859-1"  # Latin 1


def convert_file(file_path: str) -> str:
    with open(file_path, "rb") as file:
        font = reader.read_bdf(file)

    font_name = file_path.replace("/", "").replace(".", "").replace("bdf", "").replace("-", "")

    output: str = f"\nstatic const ILI9341_GlyphDef ILI9341_Font_{font_name}_Glyphs[] = {{\n"
    print(f"extern ILI9341_FontDef ILI9341_Font_{font_name};")

    default_char = int(font.properties.get(b"DEFAULT_CHAR") or 32)

    for codepoint in CODEPOINTS:
        # Override 0x7F (del) to use the default char, since 0x7F is the fallback codepoint for the renderer
        if codepoint == 0x7F:
            glyph = font.get(default_char)
        else:
            glyph = font.get(codepoint) or font.get(default_char)

        if glyph is None:
            raise ValueError(
                f"Glyph for codepoint {codepoint} not found in font {file_path} and no default char is available."
            )

        bbX = glyph.bbX
        bbY = glyph.bbY
        bbW = glyph.bbW
        bbH = glyph.bbH

        data = list(reversed(glyph.data))

        horizontal_start = bbW
        horizontal_end = 0
        for row in data:
            if row == 0:
                continue
            start = 0
            while (row & (1 << (bbW - 1 - start))) == 0:
                start += 1
            end = bbW
            while (row & (1 << (bbW - end))) == 0:
                end -= 1
            if start < horizontal_start:
                horizontal_start = start
            if end > horizontal_end:
                horizontal_end = end

        if horizontal_start >= horizontal_end:
            horizontal_start = 0
            horizontal_end = 0

        for i in range(len(data)):
            data[i] = (data[i] >> (bbW - horizontal_end)) & (
                (1 << (horizontal_end - horizontal_start + 1)) - 1
            )

        bbX += horizontal_start
        bbW = horizontal_end - horizontal_start

        while len(data) > 0 and data[0] == 0:
            data.pop(0)
            bbH -= 1
        while len(data) > 0 and data[-1] == 0:
            data.pop(-1)
            bbY += 1
            bbH -= 1

        if len(data) == 0:
            bbX = 0
            bbY = 0
            bbW = 0
            bbH = 0

        long = 0
        for row in data:
            long = (long << bbW) | row
        long <<= (-(bbW * bbH)) % 8

        longBytes = long.to_bytes((bbW * bbH + 7) // 8, "big")
        int_array = list(longBytes)

        try:
            c = bytes([codepoint]).decode(ENCODING)
        except UnicodeDecodeError:
            c = "�"

        if not c.isprintable() and c not in ("\u00a0", "\u00ad"):
            c = "�"

        output += " " * 4
        output += f"{{{bbX:3d}, {bbY:3d}, {bbW:2d}, {bbH:2d}, {glyph.advance:2d}, "
        if len(int_array) > 0:
            output += "(const uint8_t[]){"
            output += ", ".join([f"0x{i:02X}" for i in int_array])
            output += "}"
        else:
            output += "NULL"
        output += f"}}, /* {c} */\n"

    output += "};\n"
    output += f"const ILI9341_FontDef ILI9341_Font_{font_name} = "
    output += f"{{ 0x{CODEPOINT_START:02X}, 0x{CODEPOINT_END:02X}, "
    output += f"{int(font.properties.get(b'AVERAGE_WIDTH') or font[65].bbW)}, "
    output += f"{int(font.properties.get(b'FONT_ASCENT') or font[65].bbH)}, "
    output += f"{int(font.properties.get(b'FONT_DESCENT') or 0)}, "
    output += f"ILI9341_Font_{font_name}_Glyphs }};\n"

    return output


def main() -> None:
    with open("ili9341_fonts.c", "w", encoding="utf-8") as file:
        print("// Header declarations")
        file.write('#include "ili9341_fonts.h"\n')
        for file_path in natsorted(os.listdir(".")):
            if not file_path.endswith(".bdf"):
                continue
            file.write(convert_file(file_path))


if __name__ == "__main__":
    main()
