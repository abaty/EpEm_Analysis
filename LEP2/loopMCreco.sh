folder="/data/flowex/MCsamples/LEP2_MC_MAIN/LEP2_MC/GGUD"
filelist="/data/flowex/MCsamples/LEP2_MC_MAIN/Lists_MC/LEP2MCGGUDReco.list"
output="cleaned_LEP2MCGGUD-Reco-all.aleph"
suffix="LEP2MCGGUD*recons_aftercut"

rm $filelist
rm $output
rm ROOTfiles/$output.root

cd $folder
rm cleaned*
ls $suffix* >> $filelist
cd ../../
mv $folder/$filelist $filelist

while read F  ; do
        echo $F
          rm $folder/cleaned_$F
          rm ROOTfiles/cleaned_$F.root
          cp $folder/$F  $folder/cleaned_$F

          sed -i ''  's/px=//' $folder/cleaned_$F
          sed -i ''  's/py=//' $folder/cleaned_$F
          sed -i ''  's/pz=//' $folder/cleaned_$F
          sed -i ''  's/m=//' $folder/cleaned_$F
          sed -i ''  's/charge//' $folder/cleaned_$F
          sed -i ''  's/pwflag//' $folder/cleaned_$F
          awk '!/END_EVENT/' $folder/cleaned_$F >> testfile.txt && mv testfile.txt $folder/cleaned_$F
          awk '!/END_FILE/' $folder/cleaned_$F >> testfile.txt && mv testfile.txt $folder/cleaned_$F
          sed -i ''  's/EVENT//' $folder/cleaned_$F
          sed -i ''  's/ECM =//' $folder/cleaned_$F
          sed -i ''  's/GEV//' $folder/cleaned_$F
          sed -i ''  's/MC_RECO RUN =/   -999. -999. -999./' $folder/cleaned_$F
           
done <$filelist


cat $folder/cleaned_$suffix* >> $output
echo "-999. -999. -999. -999. -999. -999." >> $output

g++ scan.cc $(root-config --cflags --libs) -g -o scan.exe 
./scan.exe $output
rm scan.exe
