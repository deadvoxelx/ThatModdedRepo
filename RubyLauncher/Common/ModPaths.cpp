#include "Common/ModPaths.h"

#include "Loader.h"

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

namespace
{
	std::wstring g_modsRootWide;
	std::string g_modsRootNarrow;

	std::wstring executableDirectory()
	{
#ifdef _WIN32
		WCHAR buffer[MAX_PATH] = {};
		DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);

		if (length > 0 && length < MAX_PATH)
		{
			std::wstring path(buffer, length);
			size_t slash = path.find_last_of(L"\\/");

			if (slash != std::wstring::npos) return path.substr(0, slash);
		}
#endif
		return L".";
	}

	void resolveOnce()
	{
		if (!g_modsRootWide.empty())
			return;

		g_modsRootWide = executableDirectory() + L"\\mods";
		g_modsRootNarrow = RubyPaths::toNarrow(g_modsRootWide);
	}

	bool isSafeFolderName(const std::string &folderName)
	{
		if (folderName.empty() || folderName == "." || folderName == "..") return false;

		return folderName.find_first_of("/\\:") == std::string::npos;
	}

	bool isInsideFolder(const fs::path &candidate, const fs::path &folder)
	{
		std::error_code error;
		const fs::path canonicalCandidate = fs::weakly_canonical(candidate, error);

		if (error) return false;

		const fs::path canonicalFolder = fs::weakly_canonical(folder, error);

		if (error) return false;

		const fs::path relative = canonicalCandidate.lexically_relative(canonicalFolder);

		if (relative.empty()) return false;

		const auto first = relative.begin();

		return first != relative.end() && *first != "..";
	}
}

const std::string &RubyPaths::modsRoot()
{
	resolveOnce();
	return g_modsRootNarrow;
}

const std::wstring &RubyPaths::modsRootW()
{
	resolveOnce();
	return g_modsRootWide;
}

void RubyPaths::ensureModsFolderExists()
{
	std::error_code error;
	fs::create_directories(modsRoot(), error);

	if (error)
	{
		Loader::_debugPrint("couldnt create the Mods folder at '" + modsRoot() + "': " + error.message());
	}
}

std::string RubyPaths::modScriptPath(const std::string &folderName, const std::string &entry)
{
	if (!isSafeFolderName(folderName) || entry.empty()) return std::string();

	const fs::path folder = fs::path(modsRoot()) / folderName;
	const fs::path script = folder / entry;

	if (!isInsideFolder(script, folder)) return std::string();

	return script.string();
}

std::string RubyPaths::modRequirePath(const std::string &folderName)
{
	if (!isSafeFolderName(folderName)) return std::string();

	const fs::path folder = fs::path(modsRoot()) / folderName;

	return (folder / "?.lua").string() + ";" + (folder / "?" / "init.lua").string();
}

std::string RubyPaths::resolveModTexturePath(const std::string &folderName, const std::string &texturePath)
{
	if (!isSafeFolderName(folderName) || texturePath.empty()) return std::string();

	const fs::path folder = fs::path(modsRoot()) / folderName;
	const fs::path given(texturePath);

	std::vector<fs::path> candidates;
	candidates.push_back(folder / given);

	if (given.has_extension())
	{
		candidates.push_back(folder / "res" / given);
	}
	else
	{
		candidates.push_back(folder / (given.string() + ".png"));
		candidates.push_back(folder / "res" / (given.string() + ".png"));
	}

	for (const fs::path &candidate : candidates)
	{
		if (!isInsideFolder(candidate, folder)) continue;

		std::error_code error;

		if (fs::is_regular_file(candidate, error)) return candidate.string();
	}

	return std::string();
}

std::wstring RubyPaths::toWide(const std::string &text)
{
	return std::wstring(text.begin(), text.end());
}

std::string RubyPaths::toNarrow(const std::wstring &text)
{
	return std::string(text.begin(), text.end());
}
