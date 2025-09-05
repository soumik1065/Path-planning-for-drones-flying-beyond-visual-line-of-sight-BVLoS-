#include<stdio.h>
#include<math.h>
//#include<conio.h>

float shelter[10][10], beta, alpha, E_imp,v_imp;

int main()
{
    //float Pfatality,k,alpha,beta,Eimp,m,s,x,u,h,a,v;
	int n, i, j, no_fly[10][10];
	float event, impact, obstacle[10][10], fatality[10][10], casualty[10][10], risk[10][10], h_threshold,u,m,g;
	
	float p_impact();
	float p_fatality(int, int);
	
    //Initialization.....
	printf("Generating Risk Map (N X N)");
	printf("\n\t\tEnter N (<10):");
	scanf("%d", &n);
	
	printf("\n\t\tEnter values for no fly zone....\n\t\t(-1:flight not allowed; 0:flight allowed)");
	for (i=1; i<=n; i++)
	{
	    for (j=1; j<=n; j++)
	    {
	        printf("\n\t\tLocation (%d, %d):", i, j);
	        scanf("%d", &no_fly[i][j]);
	    }
	}

    printf("\n\t\tEnter heights for obstacles....");
	for (i=1; i<=n; i++)
	{
	    for (j=1; j<=n; j++)
	    {
	        printf("\n\t\tLocation (%d, %d):", i, j);
	        scanf("%lf", &obstacle[i][j]);
	    }
	}
	printf("\n\t\tEnter threshold height:");
    scanf("%lf", &h_threshold);
    
    printf("\n\t\tEnter sheltering factors....\n\t\t(0:No obstacles, 2.5:sparse tree, 5:vehicles & low buildings, 7.5:high buildings, 10:industrial building)");
    for (i=1; i<=n; i++)
	{
	    for (j=1; j<=n; j++)
	    {
	        printf("\n\t\tLocation (%d, %d):", i, j);
	        scanf("%lf", &shelter[i][j]);
	    }
	}
	
	//v_imp value calculation
	printf("\n\t\tEnter values of u (19.4444m/s): ");
	scanf("%lf", &u); 

	printf("\n\t\tEnter values of g (9.8m/s^2): ");
	scanf("%lf", &g);

	v_imp=sqrt((u*u)+(2*g*h_threshold));
	printf("The value of final impact velocity is: %lf\n", v_imp);
	
	//E_imp calculation
     printf("\n\t\tEnter the mass of uav(3.75kg): ");
     scanf("%lf", &m);
	E_imp=0.5*m*v_imp*v_imp;
	printf("The value of kinetic energy of impact E_imp: %lf\n", E_imp);
	
	
	//Risk map generation......
	
	printf("\n\t\tEnter event(crash) probability: ");
    scanf("%lf", &event);

	printf("Calculatig P_impact....");
	impact=p_impact();
	printf("\n\t\tP_impact: %lf", impact);
	
	printf("\n\t\tCalculatig P_fatality....");
	
	printf("\n\t\tEnter value of impact energy beta (34J):");
    scanf("%lf", &beta);
    printf("\n\t\tEnter value of impact energy alpha (100KJ):");
    scanf("%lf", &alpha);
   // printf("\n\t\tEnter kinetic energy of impact (250J):");
    //scanf("%f", &E_imp);
    
	for (i=1; i<=n; i++)
	{
	    for (j=1; j<=n; j++)
	    {
	        fatality[i][j]=p_fatality(i, j);
	        printf("\n\t\tP_fatality(%d, %d):%lf", i, j, fatality[i][j]);
	        casualty[i][j]=event*impact*p_fatality(i, j);
	        
	        if (no_fly[i][j]==-1 || obstacle[i][j]>=h_threshold)
	            risk[i][j]=-1;
	        else
	            risk[i][j]=casualty[i][j];
	        printf("\n\t\tRisk (%d, %d): %lf", i, j, risk[i][j]);     
	    }
	}
	printf("\n\t\tRisk map generation completed.");
}


float p_impact()
{
    float impact, population, r_p, r_uav, h_p, radian, a_exp, pi=3.14;
    
    printf("\n\t\tEnter population density (6900 people/km^2):");
    scanf("%lf", &population);
    printf("Enter average radius of a person(0.248m):");
    scanf("%lf", &r_p);
    printf("\n\t\tEnter radius of UAV(0.88m):");
    scanf("%lf", &r_uav);
    printf("\n\t\tEnter average height of a person(1.587m):");
    scanf("%lf", &h_p);
    printf("Enter impact angle(in radians):");
    scanf("%lf", &radian);
    
    a_exp=2*(r_p+r_uav)*h_p/tan(radian)+ pi*pow((r_uav+r_p),2);
    impact=population*a_exp;
    return(impact);
}

float p_fatality(int x, int y)
{
    float fatality, k, temp;
    
    temp=pow((beta/E_imp),(3/shelter[x][y]));
    if (temp<1) k=temp;   
    else k=1;
	
    fatality=(1-k)/( 1-2*k+sqrt(alpha/beta)*pow(beta/E_imp,3/shelter[x][y]));
    return(fatality);
} 

