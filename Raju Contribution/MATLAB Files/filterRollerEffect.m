function roller_removed = filterRollerEffect(resultant,roller_resultantAdjust,roller_type)
% Depending on the type of roller, it removes the effect of roller. It
% substracts the roller effect vector from resultant vector

    roller_removed=zeros(size(resultant)); %#codegen   
    if strcmp(roller_type,'conditional')==1 %Temco test roller
        roller_removed=resultant-roller_resultantAdjust;  
    end
    if strcmp(roller_type,'always')==1 %Labbat Roller
        a=80;
        b=105;
        roller_resultantAdjust =a+(b-a).*rand(size(resultant));
        roller_removed=resultant-roller_resultantAdjust;
    end

end