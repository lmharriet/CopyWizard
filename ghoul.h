
#include "monster.h"
class ghoul : public monster
{
private:
	int speedUpCount;
	bool isSpeedUp;
	bool isIdle;
public:
	void addInit()override;
	void update() override;

	void stateImageRender() override;
	void stateImage(int indexX_L, int indexY_L, int indexX_R, int indexY_R);

	void stateIDLE() override;
	void stateATK()	override;
	void stateDIE()	override;
};
