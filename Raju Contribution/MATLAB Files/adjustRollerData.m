function roller_resultantMat=adjustRollerData(roller_resultant,n)
% It appends or delets the roller resultant matrix so that output matrix
% size will be equal to roller resultant pre defined data matrix 

m=size(roller_resultant,1);%#codegen
% roller_resultantMat=zeros(size(roller_resultant));
roller_resultantMat=zeros(238,1);

if m>n || m==n
    roller_resultantMat=roller_resultant(1:n,1);
elseif m<n
    roller_resultantMat(1:m,1)=roller_resultant;
    roller_resultantMat(m+1:n,1)=sort(rand(n-m,1)*roller_resultant(m,1),'descend');    
end
% disp(m);
% disp(n);
end


