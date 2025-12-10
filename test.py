import plotly.graph_objects as go

# 1. Define the Categories (Nodes) and their 3D positions
categories = {
    'Set': {'pos': (0, 0, -1), 'color': 'grey', 'symbol': 'diamond'},
    'Top': {'pos': (-2, 2, 1), 'color': 'blue', 'symbol': 'circle'},
    'Grp': {'pos': (2, 2, 1), 'color': 'red', 'symbol': 'square'},
    'Ab': {'pos': (2, 0, 1), 'color': 'orange', 'symbol': 'square'},
    'Vect_k': {'pos': (2, -2, 1), 'color': 'green', 'symbol': 'cross'},
    'Ring': {'pos': (0, 3, 1), 'color': 'purple', 'symbol': 'triangle-up'}
}

# 2. Define the Functors (Edges) connecting the Categories
functors = [
    ('Top', 'Set', 'U (Forgetful)'),
    ('Grp', 'Set', 'U (Forgetful)'),
    ('Vect_k', 'Set', 'U (Forgetful)'),
    ('Ring', 'Set', 'U (Forgetful)'),
    ('Ab', 'Grp', 'Inclusion'), 
    ('Set', 'Grp', 'F (Free Group)'),
    ('Set', 'Vect_k', 'F (Free Vector Space)'),
    ('Top', 'Grp', 'π₁ (Fundamental Group)'),
    ('Top', 'Ab', 'Hₙ (Homology)'),
    ('Ring', 'Grp', 'Group of Units'),
]

# --- Create Plotly Figure ---

# 3. Create traces for the nodes (categories)
node_x = [data['pos'][0] for data in categories.values()]
node_y = [data['pos'][1] for data in categories.values()]
node_z = [data['pos'][2] for data in categories.values()]
node_text = list(categories.keys())
node_colors = [data['color'] for data in categories.values()]

node_trace = go.Scatter3d(
    x=node_x, y=node_y, z=node_z,
    mode='markers+text',
    text=node_text,
    textposition='top center',
    marker=dict(size=12, color=node_colors, opacity=0.8),
    hoverinfo='text',
    hovertext=node_text
)

# 4. Create traces for the edges (functors)
edge_x, edge_y, edge_z = [], [], []
edge_labels = []
edge_label_pos_x, edge_label_pos_y, edge_label_pos_z = [], [], []

for start, end, name in functors:
    x0, y0, z0 = categories[start]['pos']
    x1, y1, z1 = categories[end]['pos']
    edge_x.extend([x0, x1, None])
    edge_y.extend([y0, y1, None])
    edge_z.extend([z0, z1, None])
    edge_label_pos_x.append((x0 + x1) / 2)
    edge_label_pos_y.append((y0 + y1) / 2)
    edge_label_pos_z.append((z0 + z1) / 2)
    edge_labels.append(name)

edge_trace = go.Scatter3d(
    x=edge_x, y=edge_y, z=edge_z,
    mode='lines',
    line=dict(width=2, color='darkgrey'),
    hoverinfo='none'
)

label_trace = go.Scatter3d(
    x=edge_label_pos_x, y=edge_label_pos_y, z=edge_label_pos_z,
    mode='text',
    text=edge_labels,
    hoverinfo='text',
    hovertext=edge_labels,
    textfont=dict(size=10, color='black')
)

# 5. Assemble the figure and customize the layout
fig = go.Figure(data=[node_trace, edge_trace, label_trace])
fig.update_layout(
    title='A 3D Map of Mathematical Categories and Functors',
    showlegend=False,
    scene=dict(
        xaxis=dict(showticklabels=False, title='', backgroundcolor="rgba(0,0,0,0)"),
        yaxis=dict(showticklabels=False, title='', backgroundcolor="rgba(0,0,0,0)"),
        zaxis=dict(showticklabels=False, title='', backgroundcolor="rgba(0,0,0,0)"),
    ),
    margin=dict(l=0, r=0, b=0, t=40)
)

# 6. Save the figure to an HTML file
fig.write_html('test.html')

print("Successfully saved the interactive plot to test.html")