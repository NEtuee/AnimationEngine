<Header Type="CharacterSet" Name="TestSet"/>
<Skeleton>
   <FilePath>..\Resource\bone.dat</FilePath>

   <Transforms>
        <Transform Name="Hand IK Target" Parent="Bip001">
            <Local>
                <T>0 0 1</T>
            </Local>
        </Transform>
        <Transform Name="Foot IK Target" Parent="Bip001">
            <Local>
                <T>0 0 0</T>
            </Local>
        </Transform>
        <Transform Name="Look Forward" Parent="Bip001">
            <Local>
                <T>0 0 0.8</T>
            </Local>
        </Transform>
    </Transforms>
	<IKChains>
		<IKChain Solver="CCD" Effector="Bip001 R Hand" Root="Bip001 Spine" Target="Hand IK Target"/>
		<IKChain Solver="Analytic" Effector="Bip001 R Foot" Root="Bip001 R Thigh" Target="Foot IK Target" BendTarget="Bip001 R Forearm"/>
    </IKChains>
</Skeleton>

<Animations>
	<Animation Name="run" FilePath="..\Resource\Player_Run_02.dat" IsLoop="True" Speed="1">
    </Animation>
	<Animation Name="run_L" FilePath="..\Resource\Ani_Player_Run_Tilt_L_04.dat" IsLoop="True" Speed="1">
    </Animation>
	<Animation Name="run_R" FilePath="..\Resource\Ani_Player_Run_Tilt_R_04.dat" IsLoop="True" Speed="1">
    </Animation>
	<Animation Name="idle" FilePath="..\Resource\Endomorph_Idle_Narrow.dat" IsLoop="True" Speed="1">
    </Animation>
	<Animation Name="walk" FilePath="..\Resource\Endomorph_Walk.dat" IsLoop="True" Speed="1">
    </Animation>
	<Animation Name="gunIdle" FilePath="..\Resource\Gun_Idle.dat" IsLoop="True" Speed="1">
    </Animation>
	<Animation Name="gunRun" FilePath="..\Resource\Gun_Walk_F.dat" IsLoop="True" Speed="1">
    </Animation>
	<Animation Name="gunShot" FilePath="..\Resource\Gun_Shot_C.dat" IsLoop="False" Speed="1">
    </Animation>
</Animations>

<BlendTree>
    <RootMotion Enable="True" RootBoneName="Bip01"/>

    <Variables>
        <Variable Name="boolVar" Type="Bool" DefaultValue="True"/>
        <Variable Name="boolVar2" Type="Bool" DefaultValue="True"/>
        <Variable Name="floatVar" Type="Float" DefaultValue="0"/>
        <Variable Name="floatVar2" Type="Float" DefaultValue="0"/>
    </Variables>

    <Layers ResultLayer="bodyBlend">

		<BlendLayer Name="bodyBlend" Layer1="gunRun" Layer2="gunIdle" DefaultWeight="1">

            <Additive IsAdditive="False" BasePose=""/>
            <PartialMask MaskBone="Bip001 Spine" MaskDepth="100" SmoothCount="3" RotationRoot=""/>
        </BlendLayer>

        <SourceLayer Name="gunIdle" DefaultState="default">
			<Connections>
                <Connection From="default" Condition="boolVar == false" To="shot" BlendTime="0.1"/>
                <Connection From="shot" Condition="" To="default" BlendTime="0.3"/>
            </Connections>

			<Nodes>
				<StateNode Name="default">
					<Animation Name="gunIdle"/>
				</StateNode>

				<StateNode Name="shot">
					<Animation Name="gunShot"/>
				</StateNode>
            </Nodes>
        </SourceLayer>

		<SourceLayer Name="gunRun" DefaultState="default">
			<Nodes>
				<StateNode Name="default">
					<Animation Name="walk"/>
				</StateNode>
            </Nodes>
        </SourceLayer>
    </Layers>
</BlendTree>
