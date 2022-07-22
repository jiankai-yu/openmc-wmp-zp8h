# openmc-wmp-zp8h
This version of OpenMC was developed to adjust the WMP library in neutron transport and depletion analysis. The specific absorption reaction was explicitly expressed in WMP by using residues and Vector Fitted polynomial coefficients, including capture, (n, alpha), (n, proton) and fission.
Additionally, to further improve the WMP evaluation efficiency, the Rational Approximation was implemented to replace the MIT Faddeeva function. 
