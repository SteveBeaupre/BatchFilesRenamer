ScriptName = "Random"
InitialDirectory = ""

Input = ""
Output = ""

--------------------------------------------------------------------------------------------------------------------
-- Return the char at positon i in s
--------------------------------------------------------------------------------------------------------------------
function GetChar(s, i)
	return string.sub(s, i, i)
end

--------------------------------------------------------------------------------------------------------------------
-- tell if a sub string is withing a string at a given position
--------------------------------------------------------------------------------------------------------------------
function ConcatStr(s1, s2)
	return string.format("%s%s", s1, s2)
end

--------------------------------------------------------------------------------------------------------------------
-- Return the extention of a given file name
--------------------------------------------------------------------------------------------------------------------
function GetExtention(s)
	local sLen = string.len(s)

	local found = false
	local tmp = ""
	local i = sLen
	local j = 0
	while(i > 1) do
		tmp = ""
		tmp = string.sub(s, i, i)
		if(tmp == ".") then
			found = true
			break
		end
		i = i - 1;
	end;

	tmp = ""
	if(found == true) then
		while(i <= sLen) do
			tmp = tmp..string.sub(s, i, i)
			i = i + 1;
		end;
	end;

	return tmp
end


--------------------------------------------------------------------------------------------------------------------
-- Convert the file name (Called from c++)
--------------------------------------------------------------------------------------------------------------------
function Convert(FileName)
	local FileName = Input
	Output = ""

	-- Extract the file extention
	local ext = GetExtention(FileName)

	local tmp = ""
	local i = 1;
	local r = 0
	local c = ""

	while(i <= 16) do

		local k = math.random(2)
		if(k == 1) then
			r = math.random(65,90)
			c = string.char(r)
			tmp = string.format("%s%s", tmp, c)
		end
		if(k == 2) then
			r = math.random(48,57)
			c = string.char(r)
			tmp = string.format("%s%s", tmp, c)
		end

		i = i + 1
	end

	Output = string.format("%s%s", tmp, ext)

	print(Output)
end

math.randomseed(os.time())