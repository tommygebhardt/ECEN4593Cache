close all
load traces.mat
vector = M(:,33)./M(:,35);

% Remaking the matricies
% M = [M vector];
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
x = linspace(1,1.6);
index = [1 2 3 4];
xcol = length(sjeng)-2;
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
plot(x,x, 'k--', 'LineWidth', 1.2);
plot(sjengAll2(index,xcol)./sjengAll2(index(1), xcol),...
    sjengAll2(index(1),ycol)./sjengAll2(index, ycol), '-x')
plot(sjengAll4(index,xcol)./sjengAll4(index(1),xcol),...
    sjengAll4(index(1),ycol)./sjengAll4(index, ycol), '-o')
plot(sjengAllFA(index,xcol)./sjengAllFA(index(1),xcol),...
    sjengAllFA(index(1),ycol)./sjengAllFA(index, ycol), '-^')
plot(sjengAllFAL2big(index,xcol)./sjengAllFAL2big(index(1),xcol),...
    sjengAllFAL2big(index(1),ycol)./sjengAllFAL2big(index, ycol), '-v')
plot(sjengD(index,xcol)./sjengD(index(1),xcol),...
    sjengD(index(1),ycol)./sjengD(index, ycol), '-*')
plot(sjengL12(index,xcol)./sjengL12(index(1),xcol),...
    sjengL12(index(1),ycol)./sjengL12(index, ycol), '-sq')
plot(sjengL18(index,xcol)./sjengL18(index(1),xcol),...
    sjengL18(index(1),ycol)./sjengL18(index, ycol), '-d')
plot(sjengL1s4(index,xcol)./sjengL1s4(index(1),xcol),...
    sjengL1s4(index(1),ycol)./sjengL1s4(index, ycol), '-p')
plot(sjengL1s(index,xcol)./sjengL1s(index(1),xcol),...
    sjengL1s(index(1),ycol)./sjengL1s(index, ycol), '--o')
plot(sjengL24(index,xcol)./sjengL24(index(1),xcol),...
    sjengL24(index(1),ycol)./sjengL24(index, ycol), '--sq')
plot(sjengL2big(index,xcol)./sjengL2big(index(1),xcol),...
    sjengL2big(index(1), ycol)./sjengL2big(index,ycol), '--^')

axis([1 2 1 1.6]);
legend('y = x', 'All-2way', 'All-4way', 'All-FA', 'All-FA-L2-Big', 'Default', 'L1-2way',...
    'L1-8way', 'L1-small-4way', 'L1-small', 'L2-4way', 'L2-Big','Location', 'eastoutside')

% axis([2.5 6.5 2e10 6.5e10]); grid off;
title('Cost vs. Performance Time for sjeng trace');
xlabel('Cost increase ($ new / $ for 8 bytes)')
ylabel('Performance boost (cycles original/cycles improved)')
