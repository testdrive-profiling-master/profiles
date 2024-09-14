-- Docgen emoji를 위한 font awesome 목록들
docgen.font						= {}
docgen.font.fa_solid			= {}
docgen.font.fa_solid.name		= "Font Awesome 6 Free Solid"
docgen.font.fa_solid.filename	= "fontawesome_solid.odttf"
docgen.font.fa_solid.key		= "{8B7B9185-7136-4E81-A510-522B0B235A76}"
docgen.font.fa_regular			= {}
docgen.font.fa_regular.name		= "Font Awesome 6 Free Regular"
docgen.font.fa_regular.filename	= "fontawesome_regular.odttf"
docgen.font.fa_regular.key		= "{4FBADEAB-3847-40CB-8384-CE9DC33871ED}"
docgen.font.fa_brands			= {}
docgen.font.fa_brands.name		= "Font Awesome 6 Brands Regular"
docgen.font.fa_brands.filename	= "fontawesome_brand.odttf"
docgen.font.fa_brands.key		= "{D8DFF376-466D-4D04-AB9E-98EDA7FC699F}"

docgen.font.install = function (font_id)
	local font	= docgen.font[font_id]
	if font ~= nil then
		if font.installed ~= true then
			docgen.doc:AddFont(docgen.profile_path.s .. "common/bin/codegen/docgen/" .. font.filename, font.name, font.key, font_id, false);
			font.installed	= true
		end
		return true
	else
		LOGE("Emoji font(" .. font_id .. ") is not existed.")
	end
	return false
end

docgen.font.emoji = function (str)

end
