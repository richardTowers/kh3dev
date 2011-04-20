[a,fit]=system('FitnessList');
fit=textscan(fit','%f');
fit=fit{1,1};
clf;
plot1=axes();
plot(fit,'.-', 'color', [0.5 0.5 0.5]);
xlabel('Individual'); ylabel('Fitness');
hold on;
fit2=fit(1:10*round(end/10-0.5));
fit2=reshape(fit2,10,length(fit2)/10);
% Mean:
plot(10.*(1:length(mean(fit2)))-5, mean(fit2)', '.-', 'color', 'blue');
% Max:
plot(10.*(1:length(max(fit2)))-5, max(fit2)', '.-', 'color', 'green');
% Min:
plot(10.*(1:length(min(fit2)))-5, min(fit2)', '.-', 'color', 'red');
set(plot1, 'XTick',0:10:length(fit),'XGrid', 'on');
% Trend:
trend=fit-detrend(fit);
plot(trend, '--', 'color', [.5 .5 .5]);