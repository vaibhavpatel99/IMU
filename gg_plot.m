%getting data from excel file
AcX = xlsread('IMU_data_7.xlsx','B2:B74');
AcY = xlsread('IMU_data_7.xlsx','C2:C74');

%opens the graph
figure 

for ind=2:length(AcY)
  %ind allows for one after another 
  scatter(AcY(ind),AcX(ind))
  xlim([-2,2])
  ylim([-2,2])
  %draws origin
  ax = gca;
  ax.XAxisLocation = 'origin';
  ax.YAxisLocation = 'origin';
  %allows for one after another by pausing and refreshing
  pause(0.1)
  refresh
end

    
    

