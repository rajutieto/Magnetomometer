function box_stat= checkBox(ref_speed_box,speed_box,variation)
% It detects whether the target is desired box or not.
% Here considered maximum opto ON (on our predifined range, min_speed to max_speed,)
% time is our desired box.
%INPUT ref_speed_box -> the reference speed of box, we set usual speed of
                        %normal box
%      speed_box -> speed of box calculated
%      variation -> possible variation in up and down in box speed
% OUTPUT box_stat -> 1 BOX present,-1 Nothing present and 0 Noise present
                  

min_speed=ref_speed_box-variation;%#codegen
max_speed=ref_speed_box+variation;

if speed_box>min_speed && speed_box<max_speed
    box_stat=1; %BOX present
elseif speed_box==0
    box_stat=-1; % Nothing present
else
    box_stat=0; %Noise present
end

end


