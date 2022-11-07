#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Auxiliary python wrappers for Jpp PDFs.
The wrapper classes provide a common interface to evaluate the muon and shower PDF values\n
on an event-by-event basis.
"""

from math import sqrt

from abc import ABCMeta, abstractmethod

from jppy import constants as const
from jppy.geane import JGeaneWater as JGeaneWater
from jppy.pdf import (
    JMuonPDF as JMuonPDF,
    JShowerPDF as JShowerPDF,
)

class PDF(object, metaclass=ABCMeta):
    """Abstract base class for event-by-event PDF evaluation"""
    
    def __init__(self, energy=0.0, t0=0.0):
        self._energy = energy
        self._t0 = t0

    @classmethod
    def __subclasshook__(cls, subclass):
        return (hasattr(subclass, 'energy') and
                callable(subclass.energy) and
                hasattr(subclass, 't0') and
                callable(subclass.t0) and
                hasattr(subclass, 'evaluate') and
                callable(subclass.evaluate))
    
    @property
    def energy(self):
        return self._energy

    @energy.setter
    def energy(self, value):
        self._energy = float(value)

    @property
    def t0(self):
        return self._t0

    @t0.setter
    def t0(self, value):
        self._t0 = float(value)

    @abstractmethod
    def evaluate(self, D, cd, theta, phi, t_obs):
        raise NotImplementedError

        
class MuonPDF(PDF):
    """Muon PDF evaluator"""

    def __init__(self, PDFS, energy=0.0, t0=0.0, TTS=0.0):
        """
        Constructor.
        
        Parameters
        ----------
        energy : float
            muon energy at the simulated vertex 
            (i.e. interaction vertex in the case of a neutrino interaction\n
             or the vertex corresponding to the can interception for atmospheric muons)
        t0 : float
            time corresponding to the simulated vertex [ns]
            (i.e. interaction vertex in the case of a neutrino interaction\n
             or the vertex corresponding to the can interception for atmospheric muons)
        TTS : float
            transit time spread [ns]
        """

        super().__init__(energy, t0)
        
        self._geane = JGeaneWater()
        self._pdf = JMuonPDF(PDFS, TTS=TTS)

    def evaluate(self, D, cd, theta, phi, t_obs):
        """
        Muon PDF evaluation method.

        Parameters
        ----------
        D : array[float], shape=(n,)
            Hit distances with respect to the simulated vertex\n
            (i.e. interaction vertex in the case of a neutrino interaction\n
             or the vertex corresponding to the can interception for atmospheric muons)
        cd : array[float], shape=(n,)
            angle between muon direction and PMT position
        theta : array[float], shape=(n,)
            PMT longitudinal angle [deg]
        phi : array[float], shape=(n,)
            PMT azimuthal angle [deg]
        t_obs : array[float], shape=(n,)
            Observed/Simulated hit times

        Returns
        -------
        muon pdf values : array[float], shape=(n,)
        """
        
        dz = D * cd
        R = sqrt((D + dz) * (D - dz))

        E = self._geane.get_E(self.energy, dz)
        
        t_exp = self.t0 + (dz + R * const.get_kappa_c()) * const.get_inverse_speed_of_light()
        dt = t_obs - t_exp
        
        return self._pdf.calculate(E, R, theta, phi, dt)
    

class ShowerPDF(PDF):
    """Shower PDF evaluator"""

    def __init__(self, PDFS, energy=0.0, t0=0.0, TTS=0.0):
        """
        Constructor.
        
        Parameters
        ----------
        energy : float
            shower energy [GeV]
        t0 : float
            time corresponding to shower vertex [ns]
        TTS : float
            transit time spread [ns]
        """

        super().__init__(energy, t0)
        
        self._pdf = JShowerPDF(PDFS, TTS=TTS)

    def evaluate(self, D, cd, theta, phi, t_obs):
        """
        Shower PDF evaluation method.

        Parameters
        ----------
        D : array[float], shape=(n,)
            Hit distances with respect to the shower vertex
        cd : array[float], shape=(n,)
            angle between shower direction and PMT position
        theta : array[float], shape=(n,)
            PMT longitudinal angle [deg]
        phi : array[float], shape=(n,)
            PMT azimuthal angle [deg]
        t_obs : array[float], shape=(n,)
            Observed/Simulated hit times

        Returns
        -------
        shower pdf values : array[float], shape=(n,)
        """
        
        t_exp = self.t0 + D * const.get_inverse_speed_of_light() * const.get_index_of_refraction()
        dt = t_obs - t_exp
        
        return self._pdf.calculate(self.energy, D, cd, theta, phi, dt)
