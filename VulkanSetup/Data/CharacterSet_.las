<Header Type="CharacterSet" Name="TestSet"/>
<Skeleton>
   <FilePath>..\Resource\bone.dat</FilePath>
</Skeleton>

<Animations>
    <Animation Name="weaponIn" FilePath="Resource\ani_sword_00_weaponIn.dat" IsLoop="False" Speed="1">
    </Animation>
    <Animation Name="weaponOut" FilePath="Resource\ani_sword_00_weaponOut.dat" IsLoop="False" Speed="1">
    </Animation>
    <Animation Name="swordReady" FilePath="Resource\ani_sword_00_idle.dat" IsLoop="True" Speed="1">
    </Animation>
    <Animation Name="swordIdle" FilePath="Resource\ani_sword_01_idle.dat" IsLoop="True" Speed="1">
    </Animation>
    <Animation Name="swordSwing" FilePath="Resource\ani_sword_00_moveForward_Swing.dat" IsLoop="False" Speed="1">
    </Animation>
    <Animation Name="idle" FilePath="Resource\ani_idle.dat" IsLoop="True" Speed="1">
    </Animation>
    <Animation Name="walking" FilePath="Resource\ani_walk_ing.dat" IsLoop="True" Speed="1">
    </Animation>
    <Animation Name="stop" FilePath="Resource\ani_walk_stop_l.dat" IsLoop="True" Speed="1">
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

        <BlendLayer Name="additiveBlend" Layer1="bodyBlend" Layer2="idleLayer" DefaultWeight="0.3">

            <Additive IsAdditive="True" BasePose="idle"/>
            <PartialMask MaskBone="Bip01 Spine1" MaskDepth="4" SmoothCount="4" RotationRoot=""/>
        </BlendLayer>

        <BlendLayer Name="bodyBlend" Layer1="walkLayer" Layer2="defaultLayer" DefaultWeight="1">

            <Additive IsAdditive="False" BasePose=""/>
            <PartialMask MaskBone="Bip01 Spine" MaskDepth="100" SmoothCount="3" RotationRoot="Bip01"/>
        </BlendLayer>


        <SourceLayer Name="blendSpaceTest" DefaultState="testSpace">
            <Nodes>
                <BlendSpace Name="testSpace" RowVariable="floatVar" ColumnVariable="floatVar2">
                    <Animation Name="idle" Row="0" Column="0"/>
                    <Animation Name="swordIdle" Row="10" Column="0"/>
                    <Animation Name="stop" Row="0" Column="10"/>
                    <Animation Name="swordSwing" Row="20" Column="10"/>
                </BlendSpace>
            </Nodes>
        </SourceLayer>


        <SourceLayer Name="idleLayer" DefaultState="default">
            <Nodes>
                <StateNode Name="default">
                    <Animation Name="idle"/>
                </StateNode>
            </Nodes>
        </SourceLayer>

        <SourceLayer Name="walkLayer" DefaultState="idle">
            <Connections>
                <Connection From="idle" Condition="boolVar2 == false" To="walking" BlendTime="0.5"/>
                <Connection From="walking" Condition="boolVar2 == true" To="idle" BlendTime="0.5"/>
            </Connections>

            <Nodes>
                <StateNode Name="idle">
                    <Animation Name="idle"/>
                </StateNode>
                <StateNode Name="walking">
                    <Animation Name="walking"/>
                </StateNode>
            </Nodes>
        </SourceLayer>

        <SourceLayer Name="defaultLayer" DefaultState="in">
            <Connections>
                <Connection From="idle" Condition="boolVar == false" To="out" BlendTime="0.5"/>
                <Connection From="out" Condition="" To="sidle" BlendTime="0.5"/>
                <Connection From="sidle" Condition="boolVar == true" To="sready" BlendTime=".3"/>
                <Connection From="sready" Condition="boolVar == false" To="swing" BlendTime=".5"/>
                <Connection From="swing" Condition="" To="in" BlendTime=".5"/>
                <Connection From="in" Condition="" To="idle" BlendTime=".5"/>
            </Connections>

            <Nodes>
                <StateNode Name="idle">
                    <Animation Name="idle"/>
                </StateNode>
                <StateNode Name="out">
                    <Animation Name="weaponOut"/>
                </StateNode>
                <StateNode Name="sidle">
                    <Animation Name="swordIdle"/>
                </StateNode>
                <StateNode Name="sready">
                    <Animation Name="swordReady"/>
                </StateNode>
                <StateNode Name="swing">
                    <Animation Name="swordSwing"/>
                </StateNode>
                <StateNode Name="in">
                    <Animation Name="weaponIn"/>
                </StateNode>
            </Nodes>
        </SourceLayer>

    </Layers>
</BlendTree>
