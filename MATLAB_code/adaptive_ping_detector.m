clc;clear all;
filename = '../pinger_data/l60.txt';
a = csvread(filename);
Fs = a(1);
T = 1/Fs;             

plen = 30;%cm
vsound = 150000;%cm/s
Dmax = plen/(vsound*T);%max delay possible


X1 = a(2:2:end)';
X2 = a(3:2:end)';   
X1 = X1 - mean(X1);
X2 = X2 - mean(X2);

L = length(X1);
t = (0:L-1)*T;

%% Time domain
figure(1);
subplot(211)
plot(1000*t(10000:end), X1(10000:end));
title('Time domain signal');
xlabel('t(millis)');
ylabel('X(t)');

subplot(212)
plot(1000*t(10000:end), X2(10000:end));
title('Time domain signal');
xlabel('t(millis)');
ylabel('X(t)');

%% Finding std
 
N_std = 10000;
std_dev1 = zeros(1,N_std);
std_dev2 = zeros(1,N_std);
phs = zeros(1,N_std);


Xk =  X1(10000:end-1);
Xseg = zeros(1,length(Xk));
Xch = Xk.^2; 
k = length(Xk)/N_std;
thr = 0.3;

for m = 1:N_std
temp1 = Xk(floor((m-1)*k)+1:floor(m*k));
temp3 = Xch(floor((m-1)*k)+1:floor(m*k)); 
std_dev1(m) = std(temp1);
std_dev2(m) = std(temp3);
temp2 = max(std_dev2);
phs(m) = atan(std_dev2(m)/temp2);

if phs(m) > thr
    Xseg(floor((m-1)*k)+1:floor(m*k)) = Xk(floor((m-1)*k)+1:floor(m*k))*1;
end
    
end

figure(2)
subplot(411)
plot(std_dev1)

subplot(412)
plot(std_dev2) 

subplot(413)
% plot(atan(std_dev2/max(std_dev2))) 
plot(phs)

idx_adpt = phs > thr;
subplot(414)
plot(idx_adpt)

%% Segmented Peaks


figure(3)
subplot(211)
plot(Xk)

subplot(212)
plot(Xseg)

