#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <memory>
#include <filesystem>
#include <iostream>
#include <fstream>
struct image {
	std::unique_ptr<unsigned char[]> buffer;
	int w;
	int h;
	int c;
};
image read_image(const std::filesystem::path file) {
	int x;
	int y;
	int c;
	const auto s = file.generic_string();
	const auto* p = s.c_str();
	auto* buffer = stbi_load(p, &x, &y, &c, 4);
	auto result = std::make_unique<unsigned char[]>(x * y * 4);
	std::copy(buffer, buffer + (x * y * 4), result.get());
	stbi_image_free(buffer);
	return image{std::move(result),x,y,4};
}
int main(int argc,char** argv) {
	if (argc != 2) {
		std::cout << "usage: i2b [image file]\n";
	}
	std::filesystem::path in_file = argv[1];
	auto i = read_image(argv[1]);
	{
		std::ofstream out_file(in_file.replace_extension("bin"),std::ios_base::binary | std::ios_base::out);
		for (int x = 0; x < i.w; x++) {
			for (int y = 0; y < i.h; y++) {
				for (int c = 0; c < i.c; c++) {
					out_file << i.buffer[(y * i.w) + x + c];
				}
			}
		}
	}
}