// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma de Barcelona (UAB). This work is licensed under the terms of the MIT license. For a copy, see <https://opensource.org/licenses/MIT>.

using UnrealBuildTool;
using System.Collections.Generic;

public class TrafficLightTarget : TargetRules
{
	public TrafficLightTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "TrafficLight" } );
	}
}
