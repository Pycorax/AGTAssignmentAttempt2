--[[texture_loader.lua for loading Textures]]--

-- All Textures to load should be in loadTextures()
function loadTextures(context)
	-- Mesh Textures
	loadTexture(context, "calibri", "Image//calibri.tga")
	loadTexture(context, "ground", "Image//floor.tga")

	-- HUD
	loadTexture(context, "life", "Image//HUD/life.tga")

	-- SkyBox
	loadTexture(context, "skybox_left", "Image//left.tga")
	loadTexture(context, "skybox_right", "Image//right.tga")
	loadTexture(context, "skybox_top", "Image//top.tga")
	loadTexture(context, "skybox_bottom", "Image//bottom.tga")
	loadTexture(context, "skybox_front", "Image//front.tga")
	loadTexture(context, "skybox_back", "Image//back.tga")
end
