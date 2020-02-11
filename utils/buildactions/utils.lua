-- From: http://industriousone.com/topic/oscopydir

--
-- Allows copying directories.
-- It uses the premake4 patterns (**=recursive match, *=file match)
-- NOTE: It won't copy empty directories!
-- Example: we have a file: src/test.h
--	os.copydir("src", "include") simple copy, makes include/test.h
--	os.copydir("src", "include", "*.h") makes include/test.h
--	os.copydir(".", "include", "src/*.h") makes include/src/test.h
--	os.copydir(".", "include", "**.h") makes include/src/test.h
--	os.copydir(".", "include", "**.h", true) will force it to include dir, makes include/test.h
--
-- @param src_dir
--    Source directory, which will be copied to dst_dir.
-- @param dst_dir
--    Destination directory.
-- @param filter
--    Optional, defaults to "**". Only filter matches will be copied. It can contain **(recursive) and *(filename).
-- @param single_dst_dir
--    Optional, defaults to false. Allows putting all files to dst_dir without subdirectories.
--    Only useful with recursive (**) filter.
-- @returns
--    True if successful, otherwise nil.
--
function os.copydir(src_dir, dst_dir, filter, single_dst_dir, skip_existing)
	if not os.isdir(src_dir) then error(src_dir .. " is not an existing directory!") end
	filter = filter or "**"
	src_dir = src_dir .. "/"
	print('copy "' .. src_dir .. filter .. '" to "' .. dst_dir .. '".')
	dst_dir = dst_dir .. "/"
	local dir = path.rebase(".",path.getabsolute("."), src_dir) -- root dir, relative from src_dir

	os.chdir( src_dir ) -- change current directory to src_dir
		local matches = os.matchfiles(filter)
	os.chdir( dir ) -- change current directory back to root

	local counter = 0
	local skipped = 0
	for k, v in ipairs(matches) do
		local target = iif(single_dst_dir, path.getname(v), v)
		--make sure, that directory exists or os.copyfile() fails
		os.mkdir( path.getdirectory(dst_dir .. target))
		if skip_existing and os.isfile( dst_dir .. target ) then
			skipped = skipped + 1
		else
			if os.copyfile( src_dir .. v, dst_dir .. target) then
				counter = counter + 1
			end
		end
	end

	if counter + skipped == #matches then
		if counter ~= 0 then
			print( counter .. " files copied.")
		end
		if skipped ~= 0 then
			print( skipped .. " existing files not updated.")
		end
		return true
	else
		print( "Error: " .. #matches - counter - skipped .. "/" .. #matches .. " files failed to copy.")
		return nil
	end
end

function os.remove_wildcard(path)
	for k, path in ipairs(table.merge(os.matchfiles(path), os.matchdirs(path))) do
		if os.isfile(path) then
			os.remove(path)
		else
			os.rmdir(path)
		end
	end
end

function os.expanddir_wildcard(from, to)
	local dir = os.matchdirs(from)[1]
	if not dir then return end
	
	-- TODO: Optimize this
	os.copydir(dir, to)
	os.rmdir(dir)
end

function os.md5_file(path)
	if os.host() == "windows" then
		local s, errc = os.outputof(string.format("CertUtil -hashfile \"%s\" MD5", path))
		return (s:match("\n(.*)\n(.*)") or ""):gsub(" ", "")
	else
		return os.outputof(string.format("md5sum \"%s\" | awk '{ print $1 }'", path))
	end
end

function os.extract_archive(archive_path, target_path, override)
	local flags = override and "-aoa" or "-aos"

	if os.host() == "windows" then
		os.executef("call \"utils\\7z\\7za.exe\" x \"%s\" %s -o\"%s\"", archive_path, flags, target_path)
	else
		os.executef("7z x \"%s\" %s -o\"%s\"", archive_path, flags, target_path)
	end
end

function http.download_print_errors(url, file, options)
	local result_str, response_code = http.download(url, file, options)
	if result_str ~= "OK" then
		print( "\nERROR: Failed to download " .. url .. "\n" .. result_str .. " (" .. response_code .. ")" )
		if response_code == 0 then
			-- No response code means server was unreachable
			print( "Check premake5 is not blocked by firewall rules" )
		end
		return false
	end
	return true
end
