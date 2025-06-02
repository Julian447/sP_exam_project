import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('data.csv')

plt.figure(figsize=(12, 4))

plt.title('Seihr')

for col in df.columns:
    if col != 'time':
        plt.plot(df['time'], df[col], label=col)

plt.xlabel('Time')
plt.ylabel('Value')

plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15),
           ncol=5, fontsize='small')

plt.grid(True, linestyle='--', alpha=0.5)

plt.tight_layout()

plt.savefig('seihr.png', dpi=150)
plt.show()
