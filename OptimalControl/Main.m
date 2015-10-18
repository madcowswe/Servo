%Params
AccelPerA = 1000;
phaseR = 0.050;
Ts = 0.001;
N = 100;
thetaFinal = 150;
x0 = [0;0];

%system definition
%X = [theta; omega]
Ac = [0 1;
     0 0];
Bc = [0;
     AccelPerA];
C = 0;
D = 0;

SYSC = ss(Ac, Bc, [], []);
SYSD = c2d(SYSC, Ts, 'zoh');
[Phi, Gamma] = predictionmatrices(SYSD.a, SYSD.b, SYSD.c, N);

Df = Gamma(end-1:end,:);
ff = [thetaFinal; 0];

H = 2*eye(N)*(3/2)*phaseR*Ts;
[u, fval] = quadprog(H,[],[],[],Df,ff);