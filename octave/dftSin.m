N=8;
fs=1;

n = [0:N-1]; % row
t = [0:0.01:N]; % interpolated
k=fliplr(n)' - N/2;
fk = k*fs/N;
wk = 2*pi*fk;
clf;
for i=1:N
  subplot(N,2,2*i-1);
  plot(t,cos(wk(i)*t))
  axis([0,8,-1,1]);
  hold on;
  plot(n,cos(wk(i)*n),'*')
  if i==1
    title('Real Part');
  end;
  ylabel(sprintf('k=%d',k(i)));
  if i==N
    xlabel('Time (samples)');
  end;
  subplot(N,2,2*i);
  plot(t,sin(wk(i)*t))
  axis([0,8,-1,1]);
  hold on;
  plot(n,sin(wk(i)*n),'*')
  ylabel(sprintf('k=%d',k(i)));
  if i==1
    title('Imaginary Part');
  end;
  if i==N
    xlabel('Time (samples)');
  end;
end
