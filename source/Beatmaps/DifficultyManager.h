#ifndef _H__DIFFICULTYMANAGER_H_
#define _H__DIFFICULTYMANAGER_H_

class DifficultyManager
{
	public:
		static uint8_t DifficultyHpDrain;
		static uint8_t DifficultyCircleSize;
		static uint8_t DifficultyOverall;
		static float SliderMultiplier;
		static float SliderTickRate;

		static uint8_t DifficultyPeppyStars;
		static float DifficultyEyupStars;

		//inline
		static uint32_t GetCircleSize()   { return circleSize[DifficultyCircleSize] * 1.2; } //possibly *1.2
		static uint32_t GetPreemptTime()  { return preemptTime[DifficultyOverall]; }
		static uint32_t GetHitWindow300() { return hitWindow300[DifficultyOverall]; }
		static uint32_t GetHitWindow100() { return hitWindow100[DifficultyOverall]; }
		static uint32_t GetHitWindow50()  { return hitWindow50[DifficultyOverall]; }
		static uint32_t GetHitWindow()    { return hitWindow50[DifficultyOverall] << 1; }
		static uint32_t GetSpinnerTime()  { return spinnerTime[DifficultyOverall]; }
		static int32_t  GetMissHpDrain()  { return missHpDrain[DifficultyHpDrain]; }
		static float    GetHpDrainRate()  { return 1.f / (110.f + (float)missHpDrain[DifficultyHpDrain]); }

	protected:
		static uint32_t preemptTime[];
		static uint32_t circleSize[];
		static uint32_t hitWindow300[];
		static uint32_t hitWindow100[];
		static uint32_t hitWindow50[];
		static uint32_t spinnerTime[];
		static int32_t missHpDrain[];
};

#endif // _H__DIFFICULTYMANAGER_H_
