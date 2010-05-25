//
// program that tests euler-2d against known source term generated from maple
//

#include <masa.h>

using namespace MASA;
using namespace std;

#define PI = acos(-1)

double SourceQ_e ( // 24
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double);


double SourceQ_u ( // should be 22
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double);


double SourceQ_v ( // 22
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double);


double SourceQ_rho ( // 22
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double,
  double);


int main()
{
  // parameters
  double param = 1.2;
  double x=.5;
  double y=.4;

  // solutions
  double ufield,ufield2;
  double vfield,vfield2;
  double efield,efield2;
  double rho,rho2;

  // initalize
  masa_init("euler-test","euler_2d");

  // set params
  masa_set_param("u_0",param);
  double u_0=param;
  masa_set_param("u_x",param);
  double u_x=param;
  masa_set_param("u_y",param);
  double u_y=param;

  masa_set_param("v_0",param);
  double v_0=param;
  masa_set_param("v_x",param);
  double v_x=param;
  masa_set_param("v_y",param);
  double v_y=param;

  masa_set_param("rho_0",param);
  double rho_0=param;
  masa_set_param("rho_x",param);
  double rho_x=param;
  masa_set_param("rho_y",param);
  double rho_y=param;

  masa_set_param("p_0",param);
  double p_0=param;
  masa_set_param("p_x",param);
  double p_x=param;
  masa_set_param("p_y",param);
  double p_y=param;

  masa_set_param("a_px",param);
  double a_px=param;
  masa_set_param("a_py",param);
  double a_py=param;

  masa_set_param("a_rhox",param);
  double a_rhox=param;
  masa_set_param("a_rhoy",param);
  double a_rhoy=param;

  masa_set_param("a_ux",param);
  double a_ux=param;
  masa_set_param("a_uy",param);
  double a_uy=param;

  masa_set_param("a_vx",param);
  double a_vx=param;
  masa_set_param("a_vy",param);
  double a_vy=param;

  masa_set_param("Gamma",param);
  double Gamma=param;
  masa_set_param("mu",param);
  double mu=param;
  masa_set_param("L",param);
  double L=param;

  // evaluate source terms (2D)
  masa_eval_u_source  (x,y,&ufield);
  masa_eval_v_source  (x,y,&vfield);
  masa_eval_e_source  (x,y,&efield);
  masa_eval_rho_source(x,y,&rho);

  ufield2   = SourceQ_u  (x,y,u_0,u_x,u_y,v_0,v_x,v_y,rho_0,rho_x,rho_y,p_0,p_x,p_y,a_px,a_py,a_rhox,a_rhoy,a_ux,a_uy,a_vx,a_vy,L);
  vfield2   = SourceQ_v  (x,y,u_0,u_x,u_y,v_0,v_x,v_y,rho_0,rho_x,rho_y,p_0,p_x,p_y,a_px,a_py,a_rhox,a_rhoy,a_ux,a_uy,a_vx,a_vy,L);
  rho2      = SourceQ_rho(x,y,u_0,u_x,u_y,v_0,v_x,v_y,rho_0,rho_x,rho_y,p_0,p_x,p_y,a_px,a_py,a_rhox,a_rhoy,a_ux,a_uy,a_vx,a_vy,L);  
  efield2   = SourceQ_e  (x,y,u_0,u_x,u_y,v_0,v_x,v_y,rho_0,rho_x,rho_y,p_0,p_x,p_y,a_px,a_py,a_rhox,a_rhoy,a_ux,a_uy,a_vx,a_vy,Gamma,mu,L);
  
  ufield=ufield-ufield2;
  vfield=vfield-vfield2;
  efield=efield-efield2;
  rho   =rho-rho2;

  cout << endl << ufield << endl << vfield << endl << efield << endl << rho;


}
