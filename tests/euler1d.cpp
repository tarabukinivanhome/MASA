// -*-c++-*-
//
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MASA - Manufactured Analytical Solutions Abstraction Library
//
// Copyright (C) 2010 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
// $Author$
// $Id$
//
// euler1d.cpp :program that tests masa against known source term
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <config.h>
#include <masa.h>
#include <math.h>
#include <limits>
#include <iostream>
#include <stdlib.h>

using namespace MASA;
using namespace std;

template<typename Scalar>
Scalar nancheck(Scalar x)
{
  if(isnan(x))
    {
      cout << "MASA REGRESSION FAILURE:: nan found!\n";
      exit(1);
    }
  return 1;
}

template<typename Scalar>
Scalar anQ_p (Scalar x,Scalar p_0,Scalar p_x,Scalar a_px,Scalar L)
{
  Scalar pi = acos(-1);
  Scalar p_exact = p_0 + p_x * cos(a_px * pi * x / L);
  return p_exact;
}
  
template<typename Scalar>
Scalar anQ_u (Scalar x,Scalar u_0,Scalar u_x,Scalar a_ux,Scalar L)
{
  Scalar pi = acos(-1);
  Scalar u_exact = u_0 + u_x * sin(a_ux * pi * x / L);
  return u_exact;
} 
 
template<typename Scalar>
Scalar anQ_rho (Scalar x,Scalar rho_0,Scalar rho_x,Scalar a_rhox,Scalar L)
{ 
  Scalar pi = acos(-1);
  Scalar rho_exact = rho_0 + rho_x * sin(a_rhox * pi * x / L);
  return rho_exact;
}

template<typename Scalar>
Scalar SourceQ_e ( // 12
  Scalar x,
  Scalar u_0,
  Scalar u_x,
  Scalar rho_0,
  Scalar rho_x,
  Scalar p_0,
  Scalar p_x,
  Scalar a_px,
  Scalar a_rhox,
  Scalar a_ux,
  Scalar Gamma,
  Scalar mu,
  Scalar L)
{
  Scalar pi = acos(-1);
  Scalar Q_e;

  Scalar RHO;
  Scalar U;
  Scalar P;
  RHO = rho_0 + rho_x * sin(a_rhox * pi * x / L);
  P = p_0 + p_x * cos(a_px * pi * x / L);
  U = u_0 + u_x * sin(a_ux * pi * x / L);
  Q_e = cos(a_rhox * pi * x / L) * pow(U, 0.3e1) * a_rhox * pi * rho_x / L / 0.2e1 + cos(a_ux * pi * x / L) * P * a_ux * pi * u_x * Gamma / L / (Gamma - 0.1e1) + 0.3e1 / 0.2e1 * cos(a_ux * pi * x / L) * RHO * U * U * a_ux * pi * u_x / L - sin(a_px * pi * x / L) * U * a_px * pi * p_x * Gamma / L / (Gamma - 0.1e1);

  return(Q_e);
}

template<typename Scalar>
Scalar SourceQ_u ( // should be 10
  Scalar x,
  Scalar u_0,
  Scalar u_x,
  Scalar rho_0,
  Scalar rho_x,
  Scalar p_0,
  Scalar p_x,
  Scalar a_px,
  Scalar a_rhox,
  Scalar a_ux,
  Scalar L)
{
  Scalar pi = acos(-1);
  Scalar Q_u;
  Scalar RHO;
  Scalar U;

  RHO = rho_0 + rho_x * sin(a_rhox * pi * x / L);
  U = u_0 + u_x * sin(a_ux * pi * x / L);

  Q_u = 0.2e1 * cos(a_ux * pi * x / L) * RHO * U * a_ux * pi * u_x / L + cos(a_rhox * pi * x / L) * U * U * a_rhox * pi * rho_x / L - sin(a_px * pi * x / L) * a_px * pi * p_x / L;

  return(Q_u);
}

template<typename Scalar>
Scalar SourceQ_rho ( // 10
  Scalar x,
  Scalar u_0,
  Scalar u_x,
  Scalar rho_0,
  Scalar rho_x,
  Scalar p_0,
  Scalar p_x,
  Scalar a_px,
  Scalar a_rhox,
  Scalar a_ux,
  Scalar L)
{
  Scalar pi = acos(-1);
  Scalar Q_rho;
  Scalar RHO;
  Scalar U;

  RHO = rho_0 + rho_x * sin(a_rhox * pi * x / L);
  U = u_0 + u_x * sin(a_ux * pi * x / L);

  Q_rho = cos(a_ux * pi * x / L) * RHO * a_ux * pi * u_x / L + cos(a_rhox * pi * x / L) * U * a_rhox * pi * rho_x / L;

  return(Q_rho);
}

template<typename Scalar>
int run_regression()
{
  
  Scalar threshold = 5 * numeric_limits<Scalar>::epsilon();

  //variables 
  Scalar u_0;
  Scalar u_x;
  Scalar rho_0;
  Scalar rho_x;
  Scalar p_0;
  Scalar p_x;
  Scalar a_px;
  Scalar a_rhox;
  Scalar a_ux;
  Scalar Gamma;
  Scalar mu;
  Scalar L;

  // parameters
  Scalar x;

  //problem size
  int nx = 200;  // number of points
  int lx=10;     // length
  Scalar dx=Scalar(lx)/Scalar(nx);

  // solutions
  Scalar ufield,ufield2,ufield3;
  Scalar efield,efield2,efield3;
  Scalar rho,rho2,rho3;
  Scalar gradx,grady,gradz,gradp,gradrho;

  Scalar u_exact,u_exact2,u_exact3;
  Scalar p_exact,p_exact2,p_exact3;
  Scalar rho_exact,rho_exact2,rho_exact3;

  // initalize
  masa_init<Scalar>("euler-test","euler_1d");

  // initialize the default parameters
  masa_init_param<Scalar>();

  // get defaults for comparison to source terms
  // get vars
  u_0 = masa_get_param<Scalar>("u_0");
  u_x = masa_get_param<Scalar>("u_x");
  rho_0 = masa_get_param<Scalar>("rho_0");
  rho_x = masa_get_param<Scalar>("rho_x");

  p_0 = masa_get_param<Scalar>("p_0");
  p_x = masa_get_param<Scalar>("p_x");

  a_px = masa_get_param<Scalar>("a_px");
  a_rhox = masa_get_param<Scalar>("a_rhox");
  a_ux = masa_get_param<Scalar>("a_ux");

  Gamma = masa_get_param<Scalar>("Gamma");
  mu    = masa_get_param<Scalar>("mu");
  L     = masa_get_param<Scalar>("L");

  // check that all terms have been initialized
  masa_sanity_check<Scalar>();

  // evaluate source terms (1D)
  for(int i=0;i<nx;i++)
    {
      x=i*dx;
      	
      //evalulate source terms
      ufield = masa_eval_u_source  <Scalar>(x);
      efield = masa_eval_e_source  <Scalar>(x);
      rho    = masa_eval_rho_source<Scalar>(x);
      
      //evaluate analytical terms
      u_exact = masa_eval_u_exact        <Scalar>(x);
      p_exact = masa_eval_p_exact        <Scalar>(x);
      rho_exact = masa_eval_rho_exact    <Scalar>(x);

      // eval gradient terms
      gradx   = masa_eval_1d_grad_u  (x);
      gradp   = masa_eval_1d_grad_p  (x);
      gradrho = masa_eval_1d_grad_rho(x);

      // get fundamental source term solution
      ufield2   = SourceQ_u  (x,u_0,u_x,rho_0,rho_x,p_0,p_x,a_px,a_rhox,a_ux,L);
      rho2      = SourceQ_rho(x,u_0,u_x,rho_0,rho_x,p_0,p_x,a_px,a_rhox,a_ux,L);
      efield2   = SourceQ_e  (x,u_0,u_x,rho_0,rho_x,p_0,p_x,a_px,a_rhox,a_ux,Gamma,mu,L);
  
      u_exact2   = anQ_u   (x,u_0,u_x,a_ux,L);
      rho_exact2 = anQ_rho (x,rho_0,rho_x,a_rhox,L);
      p_exact2   = anQ_p   (x,p_0,p_x,a_px,L);

      // test the result is roughly zero
      // choose between abs and rel error
#ifdef MASA_STRICT_REGRESSION

      ufield3 = fabs(ufield-ufield2);
      efield3 = fabs(efield-efield2);
      rho3    = fabs(rho-rho2);

      u_exact3   = fabs(u_exact-u_exact2);
      rho_exact3 = fabs(rho_exact-rho_exact2);
      p_exact3   = fabs(p_exact-p_exact2);

#else

      ufield3 = fabs(ufield-ufield2)/fabs(ufield2);
      efield3 = fabs(efield-efield2)/fabs(efield2);
      rho3    = fabs(rho-rho2)/fabs(rho2);

      u_exact3   = fabs(u_exact-u_exact2)/fabs(u_exact2);
      rho_exact3 = fabs(rho_exact-rho_exact2)/fabs(rho_exact2);
      p_exact3   = fabs(p_exact-p_exact2)/fabs(p_exact2);

#endif

      nancheck(ufield3);
      nancheck(efield3);
      nancheck(rho3);
      
      nancheck(u_exact3);
      nancheck(rho_exact3);
      nancheck(p_exact3);
      
      if(ufield3 > threshold)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
	  cout << "U Field Source Term\n";
	  cout << "Exceeded Threshold by: " << ufield << endl;
	  cout << x << " " << endl;
	  exit(1);
	}

      if(u_exact3 > threshold)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
	  cout << "U Field Analytical Term\n";
	  cout << "Exceeded Threshold by: " << u_exact << endl;
	  cout << x << " " << endl;
	  exit(1);
	}
      
      if(efield3 > threshold)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
	  cout << "Threshold Exceeded: " << efield3 << endl;
	  cout << "MASA:               " <<  efield << endl;
	  cout << "Maple:              " <<  efield2 << endl;
	  cout << x << endl;
	  exit(1);
	}

      if(p_exact3 > threshold)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
	  cout << "P Field Analytical Term\n";
	  cout << "Exceeded Threshold by: " << p_exact << endl;
	  cout << x << endl;
	  exit(1);
	}
      
      if(rho3 > threshold)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
	  cout << "RHO Source Term\n";
	  cout << "Exceeded Threshold by: " << rho << endl;
	  cout << x << endl;
	  exit(1);
	}
      
      if(rho_exact3 > threshold)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
	  cout << "RHO Analytical Term\n";
	  cout << "Exceeded Threshold by: " << rho_exact << endl;
	  cout << x << endl;
	  exit(1);
	}

      // adding a new error check: ensure physical results are coming out!
	if(0 > rho)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
   	  cout << "Initial Variables are returning non-physical results!\n";
	  cout << "RHO\n";
	  cout << rho << endl;
	  exit(1);
	}

      if(0 > rho_exact)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
   	  cout << "Initial Variables are returning non-physical results!\n";
	  cout << "RHO analytical\n";
	  exit(1);
	}

      if(0 > p_exact)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
   	  cout << "Initial Variables are returning non-physical results!\n";
	  cout << "Pressure is negative!\n";
	  exit(1);
	}

      if(0 > efield)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
   	  cout << "Initial Variables are returning non-physical results!\n";
	  cout << "Energy is negative!\n";
	  exit(1);
	}
    } // done interating 

  // tests passed
  return 0;
}

int main()
{
  int err=0;

  err += run_regression<double>();
  //err += run_regression<long double>();

  return err;
}
