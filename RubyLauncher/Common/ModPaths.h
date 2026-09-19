#pragma once

#include <string>
#include <vector>

namespace RubyPaths
{
	const std::string &modsRoot();
	const std::wstring &modsRootW();
	void ensureModsFolderExists();
	std::string modScriptPath(const std::string &folderName, const std::string &entry);
	std::string resolveModTexturePath(const std::string &folderName, const std::string &texturePath);
	std::wstring toWide(const std::string &text);
	std::string toNarrow(const std::wstring &text);
}
