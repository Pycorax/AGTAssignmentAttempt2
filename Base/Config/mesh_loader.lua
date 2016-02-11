--[[mesh_loader.lua for loading Meshes]]--

-- All Meshes to load should be in loadMeshes()
function loadMeshes(context)
	-- Basic
	loadMeshRay(context, "Ray", 1.0, 0.0, 0.0, 10.0)
	loadMeshCrosshair(context, "crosshair", 1.0, 1.0, 1.0, 0.1)
	loadMeshSphere(context, "lightball", 1.0, 0.0, 0.0, 18, 36, 1.0)
	loadMeshSphere(context, "sphere", 1.0, 0.0, 0.0, 18, 36, 0.1)

	-- Player
	loadMeshCone(context, "playerBody", 0.282, 0.568, 0.803, 36, 1.0, 1.0)
	loadMeshCone(context, "invulnHat", 0.84, 0.0, 0.027, 36, 1.0, 1.0)

	-- Enemies
	loadMeshCone(context, "humanHat", 0.0, 0.0, 0.0, 36, 1.0, 1.0)
	loadMeshSphere(context, "humanHead", 0.968, 0.937, 0.619, 12, 12, 1.0)
	loadMeshCone(context, "humanBodyLowRes", 0.83, 0.11, 0.14, 4, 1.0, 1.0)
	loadMeshCone(context, "humanBodyMedRes", 0.83, 0.11, 0.14, 8, 1.0, 1.0)
	loadMeshCone(context, "humanBodyHighRes", 0.83, 0.11, 0.14, 36, 1.0, 1.0)

	-- SkyBox

	-- HUD
	loadMeshQuad(context, "Ammo Bar", 0.65, 0.87, 0.97, 1.0)
	loadMeshQuad(context, "PAmmo Bar", 0.95, 0.396, 0.13, 1.0)
	loadMeshQuad(context, "Score Bar", 0.22, 0.71, 0.29, 1.0)
	loadMeshQuad(context, "Bar BG", 0.54, 0.54, 0.54, 1.0)
end
