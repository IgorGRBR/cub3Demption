#!/usr/bin/python
from PIL import Image
from sys import argv
from enum import Enum
from typing import List, Tuple

# Initial config
PLAYER_ROTATION = 90

class Textures(Enum):
	FLOOR_A = 'wood_floor gray_ceiling'
	WALL_A = 'gray_wall gray_wall gray_wall gray_wall'
	DOOR_HG = 'wood_floor gray_ceiling green_door_o green_door_c 1 g'
	DOOR_HO = 'wood_floor gray_ceiling orange_door_o orange_door_c 1 o'
	DOOR_HP = 'wood_floor gray_ceiling purple_door_o purple_door_c 1 p'
	DOOR_HC = 'wood_floor gray_ceiling cyan_door_o cyan_door_c 1 c'
	DOOR_VG = 'wood_floor gray_ceiling green_door_o green_door_c 0 g'
	DOOR_VO = 'wood_floor gray_ceiling orange_door_o orange_door_c 0 o'
	DOOR_VP = 'wood_floor gray_ceiling purple_door_o purple_door_c 0 p'
	DOOR_VC = 'wood_floor gray_ceiling cyan_door_o cyan_door_c 0 c'
	EXIT_A = 'metal_exit metal_exit metal_exit metal_exit'

class TileType(Enum):
	FLOOR = 'space'
	WALL = 'wall'
	DOOR = 'door'
	EXIT = 'exit'

class ObjectType(Enum):
	PLAYER = 'player'
	PROP = 'prop'
	GUNNER = 'npc gunner'
	ITEM = 'item'

class NodeDef:
	def __init__(self, ch, ttype, txts, otype = None, odata = None):
		self.char = ch
		self.tile = ttype
		self.textures = txts
		self.obj = otype
		self.odata = odata if odata != None else []

	def get_str(self):
		node_def_str = f"cub {self.char.value} {self.tile.value} {self.textures.value}"
		if self.obj:
			node_def_str += f" object {self.obj.value}"
			if self.odata:
				node_def_str += " " + " ".join(self.odata)
		return node_def_str

	def __eq__(self, other):
		# eq = self.char == other.char
		# eq = eq and (self.tile == other.tile)
		# eq = eq and (self.obj == other.obj)
		# for i in range(len(self.textures)):
		# 	eq = eq and (self.textures[i] == other.textures[i])
		# for i in range(len(self.odata)):
		# 	eq = eq and (self.odata[i] == other.odata[i])
		return self.char == other.char

	def __hash__(self):
		return hash(self.get_str())

class Nch(Enum):
	FLOOR = '0'
	WALL = '1'
	KEYCARD_G = 'C'
	KEYCARD_O = 'c'
	KEYCARD_C = 'K'
	KEYCARD_P = 'k'
	DOOR_HG = 'D'
	DOOR_HO = 'd'
	DOOR_HC = 'J'
	DOOR_HP = 'j'
	DOOR_VG = 'U'
	DOOR_VO = 'u'
	DOOR_VC = 'Y'
	DOOR_VP = 'y'
	ENEMY_U = '^'
	ENEMY_D = 'v'
	ENEMY_R = '>'
	ENEMY_L = '<'
	AMBENEMY_U = '4'
	AMBENEMY_D = 'V'
	AMBENEMY_R = '}'
	AMBENEMY_L = '{'
	AMMO = 'A'
	HEALTH = 'H'
	BARREL = 'B'
	EXIT = 'E'
	PLAYER = 'P'

NODE_RGB_MAP = {
	(16, 16, 16): NodeDef(Nch.FLOOR, TileType.FLOOR, Textures.FLOOR_A),
	(64, 64, 64): NodeDef(Nch.WALL, TileType.WALL, Textures.WALL_A),
	(100, 64, 255): None,
	(0, 0, 255): NodeDef(Nch.EXIT, TileType.EXIT, Textures.EXIT_A),
	(0, 255, 0): NodeDef(Nch.PLAYER, TileType.FLOOR, Textures.FLOOR_A, ObjectType.PLAYER, [str(PLAYER_ROTATION)]),
	(224, 16, 32): NodeDef(Nch.ENEMY_L, TileType.FLOOR, Textures.FLOOR_A, ObjectType.GUNNER, ['180', '3']),
	(224, 48, 32): NodeDef(Nch.ENEMY_R, TileType.FLOOR, Textures.FLOOR_A, ObjectType.GUNNER, ['0', '3']),
	(224, 32, 16): NodeDef(Nch.ENEMY_U, TileType.FLOOR, Textures.FLOOR_A, ObjectType.GUNNER, ['270', '3']),
	(224, 32, 48): NodeDef(Nch.ENEMY_D, TileType.FLOOR, Textures.FLOOR_A, ObjectType.GUNNER, ['90', '3']),
	(192, 16, 32): NodeDef(Nch.AMBENEMY_L, TileType.FLOOR, Textures.FLOOR_A, ObjectType.GUNNER, ['180', '0']),
	(192, 48, 32): NodeDef(Nch.AMBENEMY_R, TileType.FLOOR, Textures.FLOOR_A, ObjectType.GUNNER, ['0', '0']),
	(192, 32, 16): NodeDef(Nch.AMBENEMY_U, TileType.FLOOR, Textures.FLOOR_A, ObjectType.GUNNER, ['270', '0']),
	(192, 32, 48): NodeDef(Nch.AMBENEMY_D, TileType.FLOOR, Textures.FLOOR_A, ObjectType.GUNNER, ['90', '0']),
	(224, 160, 224): NodeDef(Nch.HEALTH, TileType.FLOOR, Textures.FLOOR_A, ObjectType.ITEM, ['health']),
	(73, 88, 60): NodeDef(Nch.AMMO, TileType.FLOOR, Textures.FLOOR_A, ObjectType.ITEM, ['ammo']),
	(128, 255, 128): NodeDef(Nch.KEYCARD_G, TileType.FLOOR, Textures.FLOOR_A, ObjectType.ITEM, ['keycard', 'g']),
	(255, 196, 128): NodeDef(Nch.KEYCARD_O, TileType.FLOOR, Textures.FLOOR_A, ObjectType.ITEM, ['keycard', 'o']),
	(160, 128, 196): NodeDef(Nch.KEYCARD_P, TileType.FLOOR, Textures.FLOOR_A, ObjectType.ITEM, ['keycard', 'p']),
	(160, 224, 224): NodeDef(Nch.KEYCARD_C, TileType.FLOOR, Textures.FLOOR_A, ObjectType.ITEM, ['keycard', 'c']),
	(100, 200, 100): NodeDef(Nch.DOOR_HG, TileType.DOOR, Textures.DOOR_HG),
	(200, 150, 100): NodeDef(Nch.DOOR_HO, TileType.DOOR, Textures.DOOR_HO),
	(124, 100, 150): NodeDef(Nch.DOOR_HP, TileType.DOOR, Textures.DOOR_HP),
	(124, 175, 175): NodeDef(Nch.DOOR_HC, TileType.DOOR, Textures.DOOR_HC),
	(90, 180, 90): NodeDef(Nch.DOOR_VG, TileType.DOOR, Textures.DOOR_VG),
	(180, 135, 90): NodeDef(Nch.DOOR_VO, TileType.DOOR, Textures.DOOR_VO),
	(110, 90, 135): NodeDef(Nch.DOOR_VP, TileType.DOOR, Textures.DOOR_VP),
	(110, 158, 158): NodeDef(Nch.DOOR_VC, TileType.DOOR, Textures.DOOR_VC),
}
NODE_RGB_MAP.setdefault(None)

class Level:
	def __init__(self, img: Image):
		self.size = img.size
		(w, h) = self.size
		self.grid: List[List[NodeDef]] = [[None for x in range(w)] for y in range(h)]
		for y in range(h):
			for x in range(w):
				color = img.getpixel((x, y))
				tile = NODE_RGB_MAP.get(color)
				if tile:
					self.grid[y][x] = tile

	def to_str(self) -> str:
		used_nodes = set()
		tilemap_str = ""
		for y in range(self.size[1]):
			for x in range(self.size[0]):
				node: NodeDef = self.grid[y][x]
				if node:
					used_nodes.add(node)
					tilemap_str += node.char.value
				else:
					tilemap_str += ' '
			tilemap_str += '\n'

		node_def_str = ""
		for n in used_nodes:
			node_def_str += n.get_str() + "\n"

		return node_def_str + "\n" + tilemap_str

def main():
	level_name = "level"
	if (len(argv) < 2):
		raise Exception("Not enough arguments")
	if (len(argv) >= 3):
		level_name = argv[2]
	im_raw = Image.open(argv[1])
	print(im_raw.format, im_raw.size, im_raw.mode)
	im = im_raw.convert('RGB')
	level: Level = Level(im)
	print(level.size)
	with open(level_name + ".cub", "w") as f:
		f.write(level.to_str())

if __name__ == "__main__":
	main()
