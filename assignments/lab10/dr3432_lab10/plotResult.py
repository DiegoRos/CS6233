import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
data = pd.read_csv('lab10results.csv')

# Extract the data for x and y axes
x = data['framesAvailable']
y = data['pageFault']

# Create the bar chart
fig, ax = plt.subplots(figsize=(10, 6))
bars = ax.bar(x, y)

# Add labels to the bars
ax.bar_label(bars, padding=3, color='black', fontsize=10)

# Set labels and title
plt.xlabel('Frames Available')
plt.ylabel('Page Faults')
plt.title('Page Faults vs Frames Available', fontweight='bold',)

# Save the plot as a PNG file
plt.savefig('lab10_results_chart.png')