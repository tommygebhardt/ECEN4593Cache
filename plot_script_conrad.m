init_config_order = {'All-2way', 'All-4way', 'All-FA-L2Big', 'All-FA',...
    'Default', 'L1-2way', 'L1-8way', 'L1-small-4way', 'L1-small',...
    'L2-4way', 'L2-Big'};

[costs, I] = sort(M(1:11, cost_col));
execs = M(1:11, exec_time_col);
execs = execs(I);
config_order = init_config_order(I);
figure;
plot(costs, execs, '-o');
text(costs, execs, config_order, 'VerticalAlignment', 'top',...
    'HorizontalAlignment', 'left');
title('Execution Time vs. Memory Cost (bzip2)');
xlabel('Memory Cost ($)');
ylabel('Execution Time (cycles)');
print(gcf, '-djpeg', 'plots/bzip2_execT_cost.jpg');

[costs, I] = sort(M(12:22, cost_col));
execs = M(12:22, exec_time_col);
execs = execs(I);
config_order = init_config_order(I);
figure;
plot(costs, execs, '-o');
text(costs, execs, config_order, 'VerticalAlignment', 'top',...
    'HorizontalAlignment', 'left');
title('Execution Time vs. Memory Cost (h264ref)');
xlabel('Memory Cost ($)');
ylabel('Execution Time (cycles)');
print(gcf, '-djpeg', 'plots/h264ref_execT_cost.jpg');

[costs, I] = sort(M(23:33, cost_col));
execs = M(23:33, exec_time_col);
execs = execs(I);
config_order = init_config_order(I);
figure;
plot(costs, execs, '-o');
text(costs, execs, config_order, 'VerticalAlignment', 'top',...
    'HorizontalAlignment', 'left');
title('Execution Time vs. Memory Cost (libquantum)');
xlabel('Memory Cost ($)');
ylabel('Execution Time (cycles)');
print(gcf, '-djpeg', 'plots/libquantum_execT_cost.jpg');

[costs, I] = sort(M(34:44, cost_col));
execs = M(34:44, exec_time_col);
execs = execs(I);
config_order = init_config_order(I);
figure;
plot(costs, execs, '-o');
text(costs, execs, config_order, 'VerticalAlignment', 'top',...
    'HorizontalAlignment', 'left');
title('Execution Time vs. Memory Cost (omnetpp)');
xlabel('Memory Cost ($)');
ylabel('Execution Time (cycles)');
print(gcf, '-djpeg', 'plots/omnetpp_execT_cost.jpg');

[costs, I] = sort(M(45:55, cost_col));
execs = M(45:55, exec_time_col);
execs = execs(I);
config_order = init_config_order(I);
figure;
plot(costs, execs, '-o');
text(costs, execs, config_order, 'VerticalAlignment', 'top',...
    'HorizontalAlignment', 'left');
title('Execution Time vs. Memory Cost (sjeng)');
xlabel('Memory Cost ($)');
ylabel('Execution Time (cycles)');
print(gcf, '-djpeg', 'plots/sjeng_execT_cost.jpg');