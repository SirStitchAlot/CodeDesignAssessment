/** TextureAtlas.cpp
 *
 * See header file for details.
 */

#include "TextureAtlas.h"

#include "Common.h"
#include "ResourceManager.h"

#include <charconv>
#include <filesystem>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

// ----- PRIVATE FUNCTIONS/DEFINITIONS -----

#define BUFF_SIZE 1024

/** Skip to the next line in the file. 
 */
void skipToNextLine(FILE *file) {
	char ignoreLine[BUFF_SIZE];
	char *line;
	do {
		line = fgets(ignoreLine, sizeof(ignoreLine), file);
	} while(line && line[strlen(line) - 1] != '\n');
}

class ParsedFrameName {
public:
	std::string spriteName;
	std::string animName;
	unsigned frameNum;

	ParsedFrameName() {
		frameNum = 0;
	}
};

/** Parses a sprite frame name into its components.
 */
static ParsedFrameName parseFrameName(const std::string_view &frameName) {
	ParsedFrameName retVal;
	const char token = '_';
	
	auto pos = frameName.find(token);
	retVal.spriteName = std::string(frameName.substr(0, pos));

	if(pos != std::string_view::npos) {
		auto startPos = pos + 1;
		pos = frameName.find(token, startPos);

		retVal.animName = std::string(frameName.substr(startPos, pos - startPos));
	}

	if(pos != std::string_view::npos) {
		auto startPos = pos + 1;
		pos = frameName.find(token, startPos);
		if(pos == std::string_view::npos) { pos = frameName.size(); }

		std::from_chars(frameName.data() + startPos, frameName.data() + pos, retVal.frameNum);
	}

	return retVal;
}


// ----- CLASS METHODS -----

TextureAtlas::TextureAtlas(const std::string &atlasFileName) {
	// Open the atlas file
	FILE *atlasFile = fopen(atlasFileName.c_str(), "rb");

	if(!atlasFile) {
		throw std::runtime_error("Couldn't open atlas file " + atlasFileName);
	}

	parseAtlas(atlasFile, atlasFileName);

	fclose(atlasFile);
}
	
TextureAtlas::~TextureAtlas() {
}
	
const TASpriteMap& TextureAtlas::getSprites() const {
	return sprites;
}

const TASprite* TextureAtlas::getSprite(const std::string &name) const {
	auto sprite = sprites.find(name);
	if(sprite != sprites.end()) {
		return &(sprite->second);
	} else {
		return nullptr;
	}
}

void TextureAtlas::parseAtlas(FILE *file, const std::string &fileName) {
	unsigned lineNum = 0;
	fs::path path = fileName;
	std::string baseDir = path.parent_path().string();

	// First parse the atlas information line
	// Atlas info: a <imagePath> <width> <height> <spriteCount> <isFont> <fontSize>
	unsigned width = 0, height = 0, spriteFramesCount = 0, isFont = 0, fontSize = 0;
	while(!feof(file) && !ferror(file)) {
		++lineNum;
		char firstChar = fgetc(file);
		if(firstChar == 'a') {
			// Found the header
			break;
		}
		
		// Keep searching...
		skipToNextLine(file);
	}

	if(feof(file) || ferror(file)) {
		throw std::runtime_error("Atlas header line not found in file " + fileName);
	}

	char texFileName[BUFF_SIZE];
	if(fscanf(file, " %" STR(BUFF_SIZE) "s %u %u %u %u %u", &texFileName, &width, &height, &spriteFramesCount, &isFont, &fontSize) != 6 || ferror(file)) {
		throw std::runtime_error(
			TextFormat("Parse error in line %d of file %s, when parsing atlas info line.", lineNum, fileName.c_str()));
	}
	skipToNextLine(file);
	
	if(!spriteFramesCount) {
		throw std::runtime_error("No sprites in atlas file " + fileName);
	}
	
	// Load the texture
	std::string fullPath = (baseDir.length() > 0) ?
		TextFormat("%s/%s", baseDir.c_str(), texFileName) :
		texFileName;
	texture = TextureManager::load(fullPath);


	// Now parse the sprites
	// Sprite info: s <nameId> <originX> <originY> <positionX> <positionY> <sourceWidth> <sourceHeight> <padding> <trimmed> <trimRecX> <trimRecY> <trimRecWidth> <trimRecHeight>
	unsigned numSpriteFrames = 0;
	while(!feof(file) && !ferror(file)) {
		++lineNum;
		
		char firstChar = fgetc(file);
		if(firstChar != 's') {
			skipToNextLine(file);
			continue;
		}

		char name[BUFF_SIZE];
		float originX, originY;
		unsigned positionX, positionY, srcWidth, srcHeight, padding, trimmed, trimRecX, trimRecY, trimRecWidth, trimRecHeight;
		if(fscanf(file, " %" STR(BUFF_SIZE) "s %f %f %u %u %u %u %u %u %u %u %u %u", &name, &originX, &originY, &positionX, &positionY, 
				&srcWidth, &srcHeight, &padding, &trimmed, &trimRecX, &trimRecY, &trimRecWidth, &trimRecHeight) != 13 || ferror(file)) {
			throw std::runtime_error(
				TextFormat("Parse error in line %d of file %s, when parsing atlas info line.", lineNum, fileName.c_str()));
		}
		skipToNextLine(file);

		// Adjust the origin if necessary for a trimmed version
		if(trimmed) {
			originX -= trimRecX;
			originY -= trimRecY;
			srcWidth = trimRecWidth;
			srcHeight = trimRecHeight;
		}

		addSpriteFrame(name, originX, originY, positionX, positionY, srcWidth, srcHeight, padding);
		++numSpriteFrames;
	}

	if(numSpriteFrames != spriteFramesCount) {
		throw std::runtime_error(TextFormat("Texture atlas file %s is corrupt. Expected %u sprites, but found %u.", fileName.c_str(), spriteFramesCount, numSpriteFrames));
	}
}

void TextureAtlas::addSpriteFrame(const char *name, int originX, int originY, 
		unsigned positionX, unsigned positionY, unsigned width, unsigned height, unsigned padding) {
	auto [spriteName, animName, frameNum] = parseFrameName(name);

	// Update/create the sprite animation frame into the atlas
	auto &sprite = sprites[spriteName];
	auto &anim = sprite.animations[animName];
	
	if(anim.frames.size() <= frameNum) {
		anim.frames.resize(frameNum + 1);
	}

	raylib::Rectangle srcRegion(positionX, positionY, width, height);
	raylib::Vector2 origin(originX - positionX, originY - positionY);

	anim.frames[frameNum] = TAFrame(srcRegion, origin);

	// Update the sprite's size
	raylib::Vector2 size(width, height);
	if(size.x > sprite.size.x) { sprite.size.x = size.x; }
	if(size.x > sprite.size.y) { sprite.size.y = size.y; }
}


TASprite::TASprite() : size(0,0) {
}

const TASpriteAnimation* TASprite::getAnimation(const std::string &name) const {
	auto animation = animations.find(name);
	if(animation != animations.end()) {
		return &(animation->second);
	} else {
		return nullptr;
	}
}

void TASpriteAnimation::drawFrame(const TextureAtlas &atlas, unsigned frameNo, const raylib::Vector2 &position, float scale) const {
	frameNo = frameNo % getNumFrames();
	frames[frameNo].draw(atlas, position, scale);
}

unsigned TASpriteAnimation::getNumFrames() const {
	return frames.size();
}


TASpriteAnimationPlayer::TASpriteAnimationPlayer(
		std::shared_ptr<const TextureAtlas> textureAtlas, const TASpriteAnimation *animation) :
		textureAtlas(textureAtlas), animation(animation) {
	frameDelay = 5.0f / 60.0f;
	frameDelayCounter = 0;
	frameIndex = 0;
}

TASpriteAnimationPlayer::~TASpriteAnimationPlayer() {
}

bool TASpriteAnimationPlayer::update(float elapsedTime, bool loop) {
	frameDelayCounter += elapsedTime;
	if(frameDelayCounter > frameDelay) {
		frameDelayCounter = 0;
		++frameIndex;

		if(frameIndex >= animation->getNumFrames()) {
			if(loop) {
				frameIndex = 0;
			} else {
				// Stay at the last frame
				--frameIndex;
				return true;
			}
		}
	}

	return false;
}

void TASpriteAnimationPlayer::draw(const raylib::Vector2 &position, float scale) {
	animation->drawFrame(*textureAtlas, frameIndex, position, scale);
}


TAFrame::TAFrame() {
}

TAFrame::TAFrame(const raylib::Rectangle srcRegion, raylib::Vector2 origin) :
	srcRegion(srcRegion), origin(origin) {
}

void TAFrame::draw(const TextureAtlas& atlas, const raylib::Vector2 &position, float scale) const {
	auto ulPosition = position - origin / scale;
	raylib::Rectangle destRect(ulPosition.x, ulPosition.y, 
		srcRegion.width / scale, srcRegion.height / scale); 
	
	atlas.texture->Draw(srcRegion, destRect, raylib::Vector2(0.0f,0.0f), 0.0f, WHITE);
}