// DAE - Group 15 - SquirrelMania
#include "MatineeCamShake.h"
UMatineeCamShake::UMatineeCamShake()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(25.0f, 35.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Yaw.Amplitude = FMath::RandRange(25.0f, 35.0f);
}