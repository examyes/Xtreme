#include "SourceLoader.h"

#include <fstream>

namespace XASM
{

CSourceCodeHolder CSourceLoader::load_file(const std::string& file_path)
{
		CSourceCodeHolder holder(file_path);

		size_t row = 1;
		std::fstream in_file(file_path.c_str());
		if (in_file)
		{
        std::string val_line;
        while (getline(in_file, val_line))
        {
            holder.append(val_line, row);
            ++row;
        }
		}

		return holder;
}

}
