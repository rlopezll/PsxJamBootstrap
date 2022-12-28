// FBX2PSX.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>
#include "file_utils.h"
#include "string_utils.h"
#include "fbx_importer.h"

struct TArgsParser {
	int argc;
	char** argv;

	bool GetStringArg(const std::string& name, std::string& out_value) const {
		for (int idx = 0; idx < argc-1; ++idx) {
			if (name == argv[idx]) {
				out_value = argv[idx+1];
				return true;
			}
		}
		return false;
	}
	bool GetBoolArg(const std::string& name, bool& out_value) const {
		for (int idx = 0; idx < argc - 1; ++idx) {
			if (name == argv[idx]) {
				out_value = atoi(argv[idx + 1]);
				return true;
			}
		}
		return false;
	}
	int GetIntArg(const std::string& name, int& out_value) const {
		for (int idx = 0; idx < argc - 1; ++idx) {
			if (name == argv[idx]) {
				out_value = atoi(argv[idx + 1]);
				return true;
			}
		}
		return false;
	}
};

std::vector<std::string> split(const std::string& s, char seperator)
{
	std::vector<std::string> output;
	std::string::size_type prev_pos = 0, pos = 0;
	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));
		output.push_back(substring);
		prev_pos = ++pos;
	}
	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
	return output;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Help commands:\n");
		printf("-fbx <input_fbx_filename>\n");
		printf("-outdir <out_folder_h>\n");
		printf("-vertex_format <out_folder_h>\n");
		printf("-vertex_format <0/1/2/3> OPTIONAL default:0\n");
		printf("    0: VERTEX\n");
		printf("    1: VERTEX_COLOR\n");
		printf("    2: VERTEX_UV -> TODO\n");
		printf("    3: VERTEX_COLOR_UV -> TODO\n");
		printf("-texture_size <x,y>\n");
		printf("-invertY <0/1> OPTIONAL default:1\n");
		printf("Example command line:\n");
		printf("    FBX2PSX.exe -fbx teapod.fbx -outdir D:\\code\\psxdev\\jam\\PsxJamBootstrap\\source -invertY 1 -vertex_format 1");
		return 0;
	}
	TArgsParser args_parser;
	args_parser.argc = argc;
	args_parser.argv = argv;
	std::string fbx_filename;
	bool bInvertyY = true;
	int oVertexFormat = (int)TImportParams::EVertexFormatOutput::VERTEX;
	TImportParams params;
	if (!args_parser.GetStringArg("-fbx", fbx_filename)) {
		printf("Error: -fbx param missing!");
		return -1;
	}
	if (!FileUtils::existsFile(fbx_filename)) {
		printf("Error: fbx filename: '%s' not exists", fbx_filename.c_str());
		return -1;
	}
	if (!args_parser.GetStringArg("-outdir", params.m_rootFolder)) {
		printf("Error: -fbx param missing!");
		return -1;
	}
	if (!FileUtils::dirExists(params.m_rootFolder)) {
		printf("Error: outdir: '%s' not exists", params.m_rootFolder.c_str());
		return -1;
	}
	std::string texture_size;
	if (args_parser.GetStringArg("-texture_size", texture_size)) {
		VStrings values = split(texture_size,',');
		if (values.size() == 2) {
			params.m_textureSize[0] = atoi(values[0].c_str());
			params.m_textureSize[1] = atoi(values[1].c_str());
		}
		else
		{
			printf("Error: texture_size param is invalid, valid format -> x,y");
			return -1;
		}
	}
	if (args_parser.GetIntArg("-vertex_format", oVertexFormat)) {
		params.m_vertexFormatOutput = (TImportParams::EVertexFormatOutput)oVertexFormat;
	}
	args_parser.GetBoolArg("-invertY", bInvertyY);
	if(bInvertyY)
		params.m_scalarVector[1] = -params.m_scalarVector[1];

	params.refreshAbsolutePaths();
	importMeshFromFBX(fbx_filename.c_str(), params);

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
