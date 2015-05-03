close all
load traces.mat
vector = M(:,41)./M(:,42);

% Remaking the matricies
M = [M vector];
bzip2 = M(1:11,:);
h264ref = M(12:22,:);
libquantum = M(23:33, :);
omnetpp = M(34:44, :);
sjeng = M(45:end, :);
sjengnormal = M(45:55, :);
sjengAll2 = M([45, 56:58],:);
sjengAll4 = M([46, 59:61],:);
sjengAllFAL2big = M([47, 62:64],:);
sjengAllFA = M([48, 65:67],:);
sjengD = M([49, 68:70],:);
sjengL12 = M([50, 71:73],:);
sjengL18 = M([51, 74:76],:);
sjengL1s4 = M([52, 77:79],:);
sjengL1s = M([53, 80:82],:);
sjengL24 = M([54, 83:85],:);
sjengL2big = M([55, 86:88],:);

% What to plot
index = [1 2 3 4];
xcol = length(sjengD);
ycol = 13;
% Plotting
figure(1)
hold on
% plot(log2(bzip2(index,xcol)), bzip2(index,ycol), '-o')
% plot(log2(h264ref(index,xcol)), h264ref(index,ycol), '-sq')
% plot(log2(libquantum(index,xcol)), libquantum(index,ycol), '-^')
% plot(log2(omnetpp(index,xcol)), omnetpp(index,ycol), '-*')
% plot(log2(sjengnormal(index,xcol)), sjengnormal(index,ycol), '-v')
% legend('bzip2', 'h264ref', 'libquantum', 'omnetpp', 'sjeng', 'Location', 'eastoutside')

plot(sjengAll2(index,xcol), sjengAll2(index(1),ycol)/sjengAll2(index(2),ycol), 'o')
plot(sjengAll4(index,xcol), sjengAll4(index(1),ycol)/sjengAll4(index(2),ycol), 'sq')
plot(sjengAllFA(index,xcol), sjengAllFA(index(1),ycol)/sjengAllFA(index(2),ycol), '^')
plot(sjengAllFAL2big(index,xcol), sjengAllFAL2big(index(1),ycol)/sjengAllFAL2big(index(2),ycol), '-*')
plot(sjengD(index,xcol), sjengD(index(1),ycol)/sjengD(index(2),ycol), 'v')
plot(sjengL12(index,xcol), sjengL12(index(1),ycol)/sjengL12(index(2),ycol), 'x')
plot(sjengL18(index,xcol), sjengL18(index(1),ycol)/sjengL18(index(2),ycol), 'sq')
plot(sjengL1s4(index,xcol), sjengL1s4(index(1),ycol)/sjengL1s4(index(2),ycol), '^')
plot(sjengL1s(index,xcol), sjengL1s(index(1),ycol)/sjengL1s(index(2),ycol), '*')
plot(sjengL24(index,xcol), sjengL24(index(1),ycol)/sjengL24(index(2),ycol), 'v')
plot(sjengL2big(index,xcol), sjengL2big(index(1),ycol)/sjengL2big(index(2),ycol), 'o')
% legend('All-2way', 'All-4way', 'All-FA', 'All-FA-L2-Big', 'Default', 'L1-2way',...
%     'L1-8way', 'L1-small-4way', 'L1-small', 'L2-4way', 'L2-Big','Location', 'eastoutside')

% axis([2.5 6.5 2e10 6.5e10]); grid off;
title('L2 Miss Rate vs. Performance Boost for sjeng trace');
xlabel('L2 Miss Rate')
ylabel('Performance boost on doubling bus bandwidth (cycles/cycles)')
