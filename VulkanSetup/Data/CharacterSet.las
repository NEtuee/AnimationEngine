<Header Type="CharacterSet" Name="TestSet"/>
<Skeleton>
   <FilePath>..\Resource\bone.dat</FilePath>
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
</Animations>

<BlendTree>
    <RootMotion Enable="True" RootBoneName="Bip01"/>

    <Variables>
        <Variable Name="boolVar" Type="Bool" DefaultValue="True"/>
        <Variable Name="boolVar2" Type="Bool" DefaultValue="True"/>
        <Variable Name="floatVar" Type="Float" DefaultValue="0"/>
        <Variable Name="floatVar2" Type="Float" DefaultValue="0"/>
    </Variables>

    <Layers ResultLayer="defaultLayer">

        <SourceLayer Name="defaultLayer" DefaultState="testSpace">
			<Nodes>
				<BlendSpace Name="testSpace" RowVariable="floatVar" ColumnVariable="floatVar2">
                    <Animation Name="idle" Row="0" Column="0"/>
					<Animation Name="walk" Row="0" Column="5"/>
                    <Animation Name="run" Row="0" Column="10"/>
                    <Animation Name="run_L" Row="-10" Column="10"/>
                    <Animation Name="run_R" Row="10" Column="10"/>
                </BlendSpace>
            </Nodes>
        </SourceLayer>
    </Layers>
</BlendTree>
