graphics_toolkit('gnuplot')
A=load('test1.txt','-ascii');
plot(A(1000:3000,2),A(1000:3000,3),'LineWidth',5 )
hold on
plot(A(1000:3000,2),A(1000:3000,4),'LineWidth',5 )
ylabel('V')
xlabel('time (ms)')
figure
dt=diff(A(:,2));
plot(dt)
xlabel('amostra')
ylabel('\Delta t')
figure
ylabel('\Delta t')
hist(dt)
xlabel('N')
[a,b]=hist(dt)