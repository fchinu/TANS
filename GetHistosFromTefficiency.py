'''
Script for saqving histos from TEfficiency
'''

import math
from ROOT import TFile, TCanvas, TTree, gPad # pylint: disable=import-error,no-name-in-module
import numpy as np

EffiFileName='outfile.root'
EffName='effvsZ'   #'effvsZ'  
Options=''

OutFileDir='eff.root'

############################################

outfile=TFile(OutFileDir,'update')
outfile.Close()


infile = TFile(EffiFileName)
Eff = infile.Get(EffName)

histopassed=Eff.GetCopyPassedHisto()
histototal=Eff.GetCopyTotalHisto()
histopassed.SetDirectory(0)
histototal.SetDirectory(0)
infile.Close()

outfile=TFile(OutFileDir,'update')
histopassed.Write(EffName+'passed')
histototal.Write(EffName+'total')
outfile.Close()

EffiFileName='outfile.root'
EffName='eff'   #'effvsZ'  
Options=''

OutFileDir='eff.root'

############################################

outfile=TFile(OutFileDir,'update')
outfile.Close()


infile = TFile(EffiFileName)
Eff = infile.Get(EffName)

histopassed=Eff.GetCopyPassedHisto()
histototal=Eff.GetCopyTotalHisto()
histopassed.SetDirectory(0)
histototal.SetDirectory(0)
infile.Close()

outfile=TFile(OutFileDir,'update')
histopassed.Write(EffName+'passed')
histototal.Write(EffName+'total')
outfile.Close()
