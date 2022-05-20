# coding: Cp866 -> koi-8r -> Cp1251
       /* ���� ���������� ������ ������� ������������� � ������� ����� ������.
           +----------------------------------+       
           |���������� ������� 4-�� ����� ��� |       
           |  ������� ������, ������ �-903    |       
           +----------------------------------+       
                  ������     * 16-12-1994             
                  ���������� * 01-08-2002
        */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

// todo ������� ������ ������������

class aa{
    /******* ������ �� ������� � ���� ��������� **********************/
    struct Sys{
        long double // ������ (�������������� ����� 3-�� ������� � �������������)
            b, // ������������ �����������
            T[4], // ���������� �������
            tau, // ����� ������������
            x_min, // �����������
            x_max, // ----||------
    // ������ (�������������� ����� 1-�� �������)
            k_dat,
            T_dat,
            dat_min,
            dat_max,
    // �������������� �������� (������������� �����)
            k_im,
            T_im,
            s_min,
            s_max,
    // ������������ ����� (���������� �����)
            k_ro,
    // ����������� ��-���������
            k_prop,
            T_iz,
            q_min,
            q_max;
    } *psys;
    UINT i,   // ����� ����� �� �������
         j,   // ����� ����� ����� ������� �������
         mi,  // �������
         nt,  // ����� ����� �������
         is;  // ����� ����� ��� �������� �� ����
    long double k[5],         // ������������ �������-����������� ���������
                a[5],         // ������������ ����������������� ���������
                r,            // ����������� �����������
                z,            // �������
                tp,           // ����� ��������
                dt;           // ����� �� �������
    // ����������� ������
    aa::aa(void){
        // ����� ���������� ����� � �������-���������� ���������
        is = 3;
        // ��������� ����� ��� ���� �� �������
        nt = 579;
        r = 0.;
    }

    /***  ������� ������������� ����������������� ��������� ������� ***/
    public: 
    void coeff_a(void){
        a[4] = psys->T[1] * psys->T[2] * psys->T[3];
        a[3] = psys->T[1] * psys->T[2] + psys->T[1] * psys->T[3] + psys->T[2] * psys->T[3];
        a[2] = psys->T[1] + psys->T[2] + psys->T[3];
        a[1] = 1.;
        a[0] = 0.;
        return;
    }

    /********  ������� ������������� �������-����������� ��������� **/
    private:
    void coeff_k(void){
        long double c = a[1] + a[2] / dt + a[3] / dt / dt + a[4] / dt / dt / dt;
        k[0] = -a[0] / c;
        k[1] = b / c;
        k[2] = a[2] / dt;
        k[2] += 2. * a[3] / dt / dt;
        k[2] += 3. * a[4] / dt / dt / dt;
        k[2] /= c;
        k[3] = -a[3] / dt / dt;
        k[3] -= 3. * a[4] / dt / dt / dt;
        k[3] /= c;
        k[4] = a[4] / dt / dt / dt / c;
        return;
    }

    /***************  ���� ������� ***********************************/
    public:
    long double cicle(void){
        long double S_graph = 0., // ������� ��� ������
                    t_r = 0.;     // �����
        // ��� �� �������
        dt = tp / (long double)(nt * is);
        // ����� ������ ������������
        UINT d = (UINT) ceil(psys->tau / dt);
        // �������� � ���������� ������ � �������� ���
        long double* x = new long double[d+4];
        for(mi = 0; mi <= d + 3; mi++)
            x[mi] = 0.;
        coeff_k();
        j = 1;
        i = 0;
        do{
            x[d+3] = k[1] * r;
            x[d+3] += k[2] * x[d+2];
            x[d+3] += k[3] * x[d+1];
            x[d+3] += k[4] * x[d];
            if(j >= is){
                i++;
                j = 1;
                // ������ �����
                // x[0]
            }
            // ������� ������� ��� ������
            S_graph += fabs(dt * (x[0] + x[1]) / 2.);
            for(mi = 1; mi <= d + 3; mi++)
                x[mi-1] = x[mi];
            j++;
            t_r += dt;
        }while(t_r <= tp);
        delete [] x;
        return S_graph;
    }

    /******* ������ ������� *********************/
    public:
    void rasgon(void){
        long double S_graph_0, // ������� ��� 1-�� ������
                    S_graph_1, // ������� ��� 2-�� ������
                    ras;       // �������� � ��������
        // �������� ����� � 3-�
        is = 3;
        // ��� ��������� �� ����� �������
        r = 1. / b;
        // ������ ������ �����
        S_graph_0 = cicle();
        do{
            // ��������� ����� ����� �������
            is *= 2;
            // ������ ������ �����
            S_graph_1 = cicle();
            // ������� ������������ �������
            ras = S_graph_1 - S_graph_0;
            // �������� ����� ������ � ������
            S_graph_0 = S_graph_1;
            // ��������
            sound(2000);
            delay(200);
            sound(3000);
            delay(500);
            nosound();
        }while(1);
        return;
    }

    /*** ���� ����������� �������� ************************/
    private:
    void cicle_u(void){
        long double dat_i,      // ������ � �������
                    dat_i_1 = 0.,
                    eps_i,      // ������ �������������
                    q,          // ������ � ����������
                    s_i,        // ��������� ������������� ������
                    s_i_1 = 0.,
                    int_ras = 0.,   // ������ �������������
                    pereregul = 0., // �����������������
                    t_r = 0.;       // ����� ����������
        // ����� �������
        dt = tp / (long double) (nt * is);
        // ����� ������ ������������
        UINT d = (UINT) ceil(psys->tau / dt);
        long double* x = new long double [d+4];
        for(mi = 0; mi <= d + 3; mi++)
           x[mi] = 0.;
        coeff_k();
        j = 1;
        i = 0;
        do{
            // ������ �������
            z = 1.;
            // ������ �������������
            eps_i = z - x[0];
            // ������ � �������
            dat_i = psys->k_dat * eps_i + dat_i_1 * psys->T_dat / dt;
            dat_i /= 1. + psys->T_dat / dt;
            if(dat_i >= psys->dat_max)
                dat_i = psys->dat_max;
            if(dat_i < psys->dat_min)
                dat_i = psys->dat_min;
            // ������ �� ��-����������
            q = (dat_i + dat_i_1) / 2.;
            q *= dt / psys->T_iz;
            q += eps_i;
            q *= psys->k_prop;
            if(q >= psys->q_max)
                q = psys->q_max;
            if(q < psys->q_min)
                q = psys->q_min;
            // ��������� ������������� ������
            s_i = dt * psys->k_im * q / psys->T_im + s_i_1;
            if(s_i >= psys->s_max)
                s_i = psys->s_max;
            if(s_i < psys->s_min)
                s_i = psys->s_min;
            // ����������� �����������
            r = psys->k_ro * pow(s_i, 0.25);
            // ������� ������� �� ���
            x[d+3] = k[1] * r;
            x[d+3] += k[2] * x[d+2];
            x[d+3] += k[3] * x[d+1];
            x[d+3] += k[4] * x[d];
            if(j >= is){
               i++;
               j = 1;
               // ������ ����� �� �������
               // x[0]
            }
            j++;
            // ������� �����������������
            if(x[0] > pereregul)
               pereregul = x[0];
            // ������� ������������ ������ �������������
            int_ras += dt * pow(eps_i, 2.);
            // �������� �� �������
            t_r += dt;
            // �������� ��� ����� �� ���� ���� �����
            for(mi = 1; mi <= d + 3; mi++)
               x[mi-1] = x[mi];
            dat_i_1 = dat_i;
            s_i_1 = s_i;
        }while(t_r <= tp);
        // �����������������
        pereregul = (long double) ceil(100. * (pereregul - x[0]));
        delete [] x;
        // ������������ ������ �������������
        // int_ras
        return;
    }

    /************* ���������� ������� **************/
    public:
    void per(void){
        do{
            cicle_u();
            // ��������
            sound(5000);
            delay(200);
            sound(4000);
            delay(500);
            nosound();
        }while('x' != getch());
        return;
    }

    /*********  ������ ������ �� �����  ******************/
    public:
    void read_dat(void){
        // ������ � "zsr_asu.dat" � ������� �����
        FILE* io = fopen(file_name, "rb");
        if(io != NULL)
            fread(psys, sizeof(Sys), 1, io);
        else{
            paa->opros_p();
            io = fopen(file_name, "wb");
            fwrite(psys, sizeof(Sys), 1, io);
        }
        fclose(io);
        return;
    }

    /******** ���������� ������ � ���� ***************************/
    public:
    void record_dat(void){
        FILE* io = fopen(file_name, "wb");
        fwrite(psys, sizeof(Sys), 1, io);
        fclose(io);
    }
} *paa;
