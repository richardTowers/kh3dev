[a,fit]=system('FitnessList');
fit=textscan(fit','%f');
fit=fit{1,1};

plot(fit);