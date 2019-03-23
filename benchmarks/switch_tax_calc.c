//
// Created by suz305 on 3/23/19.
//
// modified from http://web.csulb.edu/~hill/et286/Week%2004a%20-%20Income%20Tax%20Calculator%20Application.pdf

#include "wp_annotation.h"

double calc_tax(double taxRate, double salary) {
    double incomeTax;
    _wp_begin();
    if (salary > 0) { // determine appropriate tax rate based on yearly salary
        switch ((int) salary / 25000) {
            case 0: // yearly salary under $25,000
                taxRate = 0.15; // 15% tax rate
                break;

            case 1: // yearly salary in range $25,000-49,999
            case 2: // yearly salary in range $50,000-74,999
                taxRate = 0.25; // 25% tax rate
                break;

            case 3: // yearly salary in range $75,000-99,999
            case 4: // yearly salary in range $100,000-124,999
            case 5: // yearly salary in range $125,000-149,999
                taxRate = 0.28; // 28% tax rate
                break;
            case 6: // yearly salary in range $150,000-174,999
            case 7: // yearly salary in range $175,000-199,999
            case 8: // yearly salary in range $200,000-224,999
            case 9: // yearly salary in range $225,000-249,999
            case 10: // yearly salary in range $250,000-274,999
            case 11: // yearly salary in range $275,000-299,999
                taxRate = 0.33; // 33% tax rate
                break;

            default: // yearly salary $300,000 or higher
                taxRate = 0.35; // 35% tax rate
        } // end switch

        incomeTax = salary * taxRate; // calculate taxes
    } else {
        incomeTax = -1; // calculate taxes
    }
    _wp_end("");
    return incomeTax;
}
