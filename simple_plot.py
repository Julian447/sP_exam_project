import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV
df = pd.read_csv('data.csv')

# Plot
plt.figure(figsize=(8, 6))
plt.plot(df['time'], df['A'], label='A', color='red')
plt.plot(df['time'], df['B'], label='B', color='green')
plt.plot(df['time'], df['C'], label='C', color='blue')

plt.title('A(0)=100, B(0)=0, C=1')
plt.xlabel('time')
plt.ylabel('count')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig('simple_plot.png')
plt.show()
