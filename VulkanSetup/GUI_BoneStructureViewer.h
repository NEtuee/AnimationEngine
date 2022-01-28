#pragma once
#include "GUIBase.h"
#include <unordered_map>

class BoneStructure;
class TransformStructure;
class GUI_BoneStructureViewer : public GUIBase
{
public:
					GUI_BoneStructureViewer();
					GUI_BoneStructureViewer(const GUI_BoneStructureViewer&);
					~GUI_BoneStructureViewer();

	void			set(BoneStructure* target);
	void			select(TransformStructure* target);

	virtual void	initialize() override;
	virtual void	frame() override;
	virtual void	render() override;
private:
	void			updateToLocal();
	void			updateToTarget();

	void			renderStructure(TransformStructure* root,float depth);
private:
	BoneStructure*		_targetBoneStructure;
	TransformStructure* _selectedTransform;

	std::unordered_map<TransformStructure*, bool> _openMap;

	bool				_showWorld = false;

	float				_positionInput[3];
	float				_scaleInput[3];
	float				_rotationInput[4];
};

