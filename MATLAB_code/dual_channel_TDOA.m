clc;clear all;filename = './pinger_data/l1802.txt';

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
 
% X1 = X1(10000:end-1);
% X2 = X2(10000:end-1);

% L = length(X1);
% t = (10000:L-1)*T;

%% Frequency domain

Y1 = fft(X1);
Y2 = fft(X2);

P1 = Y1(1:L/2+1);   %abs(Y/L);
P1 = abs(P1/L);
P1(2:end-1) = 2*P1(2:end-1);
f = Fs*(0:(L/2))/L;

figure(2);
subplot(211)
plot( f(10000:end),abs(P1(10000:end)));
title('Amplitude Spectrum of X(t)');
xlabel('f(Hz)'); %101810
ylabel('|Y(f)|');


P1 = Y2(1:L/2+1);   %abs(Y/L);
P1 = abs(P1/L);
P1(2:end-1) = 2*P1(2:end-1);
f = Fs*(0:(L/2))/L;

figure(2);
subplot(212)
plot( f(10000:end),abs(P1(10000:end)));
title('Amplitude Spectrum of X(t)');
xlabel('f(Hz)');
ylabel('|Y(f)|');

%% Filtered 
% FilteredSignal = filter(Hd,X1);
% Y1_new = fft(FilteredSignal);
% 
% figure(12)
% subplot(211)
% plot(abs(Y1_new))
% subplot(212)
% plot(FilteredSignal)
%% Filter2
Y1f = zeros(1,L);
Y2f = zeros(1,L);
b = L/Fs; 

fpass1 = 77000;
fpass2 = 83000;
Y1f(floor(fpass1*b):floor(fpass2*b))= Y1(floor(fpass1*b):floor(fpass2*b));
Y1f(floor(L-floor(fpass2*b):L-floor(fpass1*b))) = Y1(floor(L-floor(fpass2*b):L-floor(fpass1*b)));
Y2f(floor(fpass1*b):floor(fpass2*b))= Y2(floor(fpass1*b):floor(fpass2*b));
Y2f(floor(L-floor(fpass2*b):L-floor(fpass1*b))) = Y2(floor(L-floor(fpass2*b):L-floor(fpass1*b)));

fpass1 = 37000;
fpass2 = 43000;
Y1f(floor(fpass1*b):floor(fpass2*b))= Y1(floor(fpass1*b):floor(fpass2*b));
Y1f(floor(L-floor(fpass2*b):L-floor(fpass1*b))) = Y1(floor(L-floor(fpass2*b):L-floor(fpass1*b)));
Y2f(floor(fpass1*b):floor(fpass2*b))= Y2(floor(fpass1*b):floor(fpass2*b));
Y2f(floor(L-floor(fpass2*b):L-floor(fpass1*b))) = Y2(floor(L-floor(fpass2*b):L-floor(fpass1*b)));
 
X1f = ifft(Y1f);
mag_X1f = abs(X1f);

X2f = ifft(Y2f);
mag_X2f = abs(X2f);

figure(3)
subplot(211)
plot(mag_X1f(100000:end-1000));
title("filtered data1 time domain");
X1grp = mag_X1f(100000:end-1000-1);
 
subplot(212)
plot(mag_X2f(100000:end-1000));
title("filtered data2 time domain")
X2grp = mag_X2f(100000:end-1000-1);

%% Debug filter
figure(4)
subplot(221)
plot(abs(Y1(10000:end-10000)))
subplot(222)
plot(abs(Y2(10000:end-10000)))
subplot(223)
temp1 = fft(X1f);
plot(abs(temp1(10000:end-10000)))
subplot(224)
temp2 = fft(X2f);
plot(abs(temp2(10000:end-10000)))
%% Smooth the curve
X1smt = X1grp;%fastsmooth(X1grp,200,5,0);
X2smt = X2grp;%fastsmooth(X2grp,200,5,0);
% figure(4)
% subplot(211)
% plot(X1smt)
% 
% subplot(212)
% plot(X2smt)

%% Peak Detector
[peakLoc1, peakMag1] = peakfinder(X1smt, 20, 100, 1, false, true);
Loc1(1) = peakLoc1(1);
Mag1(1) = peakMag1(1);
L1 = 1;
%for avoiding the prob of multiple and noisy peaks
for i = 2:length(peakLoc1)
      if(peakLoc1(i) - Loc1(L1) > 75000)
          L1 = L1+1;
          Loc1(L1) = peakLoc1(i);
          Mag1(L1) = peakMag1(i);
      end
      if((peakLoc1(i) - Loc1(L1) < 30000) && (peakMag1(i)> peakMag1(i-1)))
          Loc1(L1) = peakLoc1(i);
          Mag1(L1) = peakMag1(i);
      end
end
temp = (Loc1 - 1) * T;
figure(5)
subplot(211)
plot(t(1:length(X1smt)),X1smt,'-',temp,Mag1,'o');


[peakLoc2, peakMag2] = peakfinder(X2smt, 20, 100, 1, true, true);
Loc2(1) = peakLoc2(1);
Mag2(1) = peakMag2(1);
L2 = 1;
%for avoiding the prob of multiple and noisy peaks
for i = 2:length(peakLoc2)
      if(peakLoc2(i) - Loc2(L2) > 75000)
          L2 = L2+1;
          Loc2(L2) = peakLoc2(i);
          Mag2(L2) = peakMag2(i);
      elseif((peakLoc2(i) - Loc2(L2) < 30000) && (peakMag2(i)> peakMag2(i-1)))
          Loc2(L2) = peakLoc2(i);
          Mag2(L2) = peakMag2(i);
      end
      
end
temp = (Loc2 - 1) * T;
figure(5)
subplot(212)
plot(t(1:length(X2smt)),X2smt,'-',temp,Mag2,'o');

temp = (peakLoc1 - 1) * T;
%% Peak detector debugging
% figure(6)
% subplot(211)
% plot(t(1:length(X1smt)),X1smt,'-',temp,peakMag1,'o');
% 
% temp = (peakLoc2 - 1) * T;
% figure(6)
% subplot(212)
% plot(t(1:length(X2smt)),X2smt,'-',temp,peakMag2,'o');

%% Peak Extraction
n = length(Loc1)-2;

X1seg = zeros(n,5000); 
X2seg = zeros(n,5000);
delay = zeros(1,n);
delaysmt = zeros(1,n);%
delaysegsmt = zeros(1,n);%
for i = 2:length(Loc1)-1
    
    X1seg(i-1,:)= X1smt(round(Loc1(i)-2500):round(Loc1(i)+2500-1));
    X2seg(i-1,:) = X2smt(round(Loc1(i)-2500):round(Loc1(i)+2500-1));
    X1segsmt(i-1,:) = fastsmooth(X1seg(i-1,:),200,5,0);
    X2segsmt(i-1,:) = fastsmooth(X2seg(i-1,:),200,5,0);

    [c,lags] = xcorr(X1seg(i-1,:),X2seg(i-1,:));
    [~,temp] = max(c);
    delay(i-1) = lags(temp);
    
    csmt = fastsmooth(c,10,1,0);%
    [~,temp] = max(csmt);%
    delaysmt(i-1) = lags(temp);%
    
    [c,lags] = xcorr(X1segsmt(i-1,:),X2segsmt(i-1,:));
    [~,temp] = max(c);
    delaysegsmt(i-1) = lags(temp);
end
%% Results display
figure(7)
for i = 1:n
subplot(2,n,i)
plot(X1seg(i,:))
subplot(2,n,i+n)
plot(X2seg(i,:))
end
figure(8)
subplot(311)
plot(1:n,delay)
var(delay)/1000
mean(delay)

figure(8)
subplot(312)
plot(1:n,delaysmt)
var(delaysmt)/1000
mean(delaysmt)

figure(8)
subplot(313)
plot(1:n,delaysegsmt)
var(delaysegsmt)/1000
mean(delaysegsmt)

%% Delay correction modification
% avg = 0;
% temp = 0;
% offset = 100;
% for i = 1:n
% 
% %     if(abs(delay(i))<Dmax+offset)
%         avg = avg + delay(i);
% %         disp(avg);
%         temp = temp+1;
% %     else
%         % disp(delay(i),i);
% %         delay(i)= 0;
% %     end
% end
% 
% 
% points = avg/temp
% alpha = (vsound*points*T)/(plen);
% % angle =  acos(alpha)*180/pi
% subplot(212)
% plot(1:n,delay)

%% cross correlation debug
% [c,~] = xcorr(X1seg(2,:),X2seg(2,:));
% figure(9)
% subplot(211)
% plot(lags,c,'-')
% [~,I] = max(c);
% 
% % I-length(c)/2;
% lags(I)
% 
% csmt = fastsmooth(c,10,1,0);
% figure(9)
% subplot(212)
% plot(lags,csmt,'-')
% [~,I] = max(csmt);
% % I-length(c)/2;
% lags(I)
 

%% GCC-PHAT
% [tau_est,R,lags] = gccphat(double(X1seg(6,:)'),double(X2seg(6,:)'),Fs);
% disp(tau_est*Fs)
% figure(9)
% subplot(212)
% plot(1000*lags,real(R(:,1)))
% xlabel('Lag Times (ms)')
% ylabel('Cross-correlation')

%% Cross correlation
% 
%   temp1 = fft(X1seg(6,:));
%   temp2 = fft(X2seg(6,:));
%   R = temp1.*conj(temp2);
% %   R = exp(1i*angle(R));
%   cc = ifft(R);
%   cc = circshift(cc,length(cc)/2);
%   figure(9)
%   subplot(212)
%   plot(abs(cc))
%   
%   [~,I] = max(cc);
%   I-length(cc)/2 -1
  
%  %% Hilberts Transform
% img_X1f = hilbert(real(cc));
% analytic1 = real(cc) + i.*img_X1f;
% env1 = abs(analytic1);
% 
% figure(6)
% subplot(212)
% plot(env1);
% title("env1");
% 
% 
%   [~,I] = max(env1);
%   I-length(cc)/2