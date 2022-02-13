#include "AnimationDataRow.h"

const char positionFlag = 0b001;
const char scaleFlag = 0b010;
const char rotationFlag = 0b100;

void AnimationDataRow::serialize(Serialization* serialize, std::ostream* stream)
{
	clock_t start, end;
	double result;
	start = clock();

	serialize->write(stream, &_boneScale, sizeof(_boneScale));
	serialize->write(stream, &_frameCount, sizeof(_frameCount));
	serialize->write(stream, &_boneConut, sizeof(_boneConut));
	serialize->write(stream, &_fps, sizeof(_fps));
	serialize->write(stream, &_isFacial, sizeof(_isFacial));
	serialize->write(stream, &_isAdditive, sizeof(_isAdditive));
	serialize->write(stream, &_isSinglebone, sizeof(_isSinglebone));

	serialize->writeString(stream, _fileTag);
	//serialize->writeUnMap(stream, _bones);
	serialize->writeUnMap(stream, _hashBones);

	end = clock() - start;
	result = static_cast<double>(end);
	std::cout << "animation serialization : " << result << "ms" << std::endl;
}

void AnimationDataRow::deserialize(Serialization* serialize, std::istream* stream)
{
	clock_t start, end;
	double result;
	start = clock();

	//serialize->read(stream, );
	serialize->read(stream, &_boneScale, sizeof(_boneScale));
	serialize->read(stream, &_frameCount, sizeof(_frameCount));
	serialize->read(stream, &_boneConut, sizeof(_boneConut));
	serialize->read(stream, &_fps, sizeof(_fps));
	serialize->read(stream, &_isFacial, sizeof(_isFacial));
	serialize->read(stream, &_isAdditive, sizeof(_isAdditive));
	serialize->read(stream, &_isSinglebone, sizeof(_isSinglebone));


	serialize->readString(stream, _fileTag);
	//serialize->readUnMap(stream, _bones);
	serialize->readUnMap(stream, _hashBones);

	end = clock() - start;
	result = static_cast<double>(end);
	std::cout << "animation deserialization : " << result << "ms" << std::endl;
}

void BoneDataRow::serialize(Serialization* serialize, std::ostream* stream)
{
	serialize->writeString(stream, _name);
	serialize->writeVector(stream, _frames);
}

void BoneDataRow::deserialize(Serialization* serialize, std::istream* stream)
{
	serialize->readString(stream, _name);
	serialize->readVector(stream, _frames);
}