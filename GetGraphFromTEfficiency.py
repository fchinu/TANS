'''
Script for producing graphs from TEfficiency
'''

import math
from ROOT import TFile, TCanvas, TTree, gPad # pylint: disable=import-error,no-name-in-module
import numpy as np

EffiFileName='outfile.root'
EffName='effvsZ' #'effvsZ' #'eff'   #'effvsZ'  
Options=''

OutFileDir='eff.root'

############################################

infile = TFile(EffiFileName)
Eff = infile.Get(EffName)

Eff.Draw(Options)

graph=Eff.GetPaintedGraph()
graph.SetName(EffName)
infile.Close()

outfile=TFile(OutFileDir,'update')
graph.Write(EffName)
outfile.Close()
