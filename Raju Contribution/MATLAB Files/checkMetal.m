function metal_stat= checkMetal(cvVal,ref_cv,variation_cv)
% It checks the CV value and decides the metal presence.
% INPUT cvVal -> calculated cv value
%       ref_cv -> reference cv value for no metal case
%       variation_cv -> used for acceptable variation for tolerance
% OUTPUT metal_stat -> 1 BOX present and 0 Noise present

    max_cv=ref_cv+variation_cv; %#codegen

    if cvVal<max_cv
        metal_stat=1; %BOX present
    %     disp('CLEAN BOX-- NO METAL PRESENT');
    else
        metal_stat=0; %Noise present
    %     disp('CONTAMINATED BOX--METAL DETECTED');
    end
end
