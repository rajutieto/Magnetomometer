function largenoisemetal_stat=checkLargeNoiseMetal(cvVal,max_cv)
% It checks the CV value and decides the presence of large metal around

    if cvVal>max_cv %#codegen
        largenoisemetal_stat=1; %Large metal presence
    else
        largenoisemetal_stat=0; %No large metal
    end
end