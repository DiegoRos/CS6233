import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
data = pd.read_csv('lab10results.csv')

# Extract the data for x and y axes
x = data['framesAvailable']
y = data['pageFault']

# Create the bar chart
plt.figure(figsize=(10, 6))
plt.bar(x, y)

# Set labels and title
plt.xlabel('Frames Available')
plt.ylabel('Number of Page Faults')
plt.title('Lab 10 Results')

# Save the plot as a PNG file
plt.savefig('lab10_results_chart.png')