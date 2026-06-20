import React, { useEffect, useMemo, useRef, useState } from "react";
import cytoscape from "cytoscape";
import { Activity, Binary, Network, RadioTower, Search, ShieldAlert } from "lucide-react";
import "./styles.css";

type NodeType = "Base" | "Tower" | "Drone" | "Rescue" | "Checkpoint" | "Supply" | "Enemy" | "Monitor";

type SimNode = {
  id: string;
  label: string;
  type: NodeType;
  status: string;
  threat: number;
  connectivity: number;
};

type SimEdge = {
  id: string;
  source: string;
  target: string;
  distance: number;
  risk: number;
  signal: number;
};

const nodes: SimNode[] = [
  { id: "101", label: "Alpha Base", type: "Base", status: "online", threat: 1, connectivity: 96 },
  { id: "102", label: "North Relay", type: "Tower", status: "online", threat: 2, connectivity: 91 },
  { id: "103", label: "Drone D-14", type: "Drone", status: "online", threat: 3, connectivity: 75 },
  { id: "104", label: "Rescue Unit", type: "Rescue", status: "online", threat: 1, connectivity: 68 },
  { id: "105", label: "Delta Checkpoint", type: "Checkpoint", status: "online", threat: 4, connectivity: 62 },
  { id: "106", label: "Echo Supply", type: "Supply", status: "online", threat: 2, connectivity: 88 },
  { id: "107", label: "Zone Seven", type: "Enemy", status: "monitored", threat: 8, connectivity: 35 },
  { id: "108", label: "Signal Watch", type: "Monitor", status: "online", threat: 2, connectivity: 80 }
];

const edges: SimEdge[] = [
  { id: "e1", source: "101", target: "102", distance: 6, risk: 1, signal: 94 },
  { id: "e2", source: "102", target: "103", distance: 5, risk: 2, signal: 85 },
  { id: "e3", source: "103", target: "105", distance: 8, risk: 5, signal: 70 },
  { id: "e4", source: "105", target: "107", distance: 4, risk: 8, signal: 45 },
  { id: "e5", source: "101", target: "104", distance: 7, risk: 2, signal: 82 },
  { id: "e6", source: "104", target: "106", distance: 3, risk: 1, signal: 76 },
  { id: "e7", source: "106", target: "108", distance: 5, risk: 2, signal: 89 },
  { id: "e8", source: "108", target: "102", distance: 4, risk: 1, signal: 90 }
];

function bfs(startId: string) {
  const seen = new Set<string>();
  const queue: Array<{ id: string; depth: number }> = [{ id: startId, depth: 0 }];
  const order: Array<{ id: string; depth: number }> = [];
  seen.add(startId);
  while (queue.length) {
    const current = queue.shift()!;
    order.push(current);
    edges
      .filter((e) => e.source === current.id || e.target === current.id)
      .forEach((e) => {
        const next = e.source === current.id ? e.target : e.source;
        if (!seen.has(next)) {
          seen.add(next);
          queue.push({ id: next, depth: current.depth + 1 });
        }
      });
  }
  return order;
}

function dfs(startId: string) {
  const seen = new Set<string>();
  const order: Array<{ id: string; depth: number }> = [];
  const visit = (id: string, depth: number) => {
    seen.add(id);
    order.push({ id, depth });
    edges
      .filter((e) => e.source === id || e.target === id)
      .forEach((e) => {
        const next = e.source === id ? e.target : e.source;
        if (!seen.has(next)) visit(next, depth + 1);
      });
  };
  visit(startId, 0);
  return order;
}

export default function App() {
  const cyRef = useRef<HTMLDivElement | null>(null);
  const cyInstance = useRef<cytoscape.Core | null>(null);
  const [selected, setSelected] = useState<SimNode>(nodes[0]);
  const [active, setActive] = useState<Array<{ id: string; depth: number }>>(bfs("101"));

  const metrics = useMemo(
    () => [
      ["Active Threats", 4, ShieldAlert],
      ["Connected Regions", 1, Network],
      ["Events Waiting", 6, Activity],
      ["Towers Online", 2, RadioTower],
      ["Nodes Count", nodes.length, Binary],
      ["Edges Count", edges.length, Search]
    ],
    []
  );

  useEffect(() => {
    if (!cyRef.current) return;
    const cy = cytoscape({
      container: cyRef.current,
      elements: [
        ...nodes.map((node) => ({ data: { id: node.id, label: node.label, type: node.type } })),
        ...edges.map((edge) => ({ data: { id: edge.id, source: edge.source, target: edge.target, risk: edge.risk } }))
      ],
      style: [
        { selector: "node", style: { label: "data(label)", "background-color": "#46617a", color: "#f8fafc", "font-size": 10 } },
        { selector: "edge", style: { width: 2, "line-color": "#8fa6b8", label: "risk data(risk)", "font-size": 8 } },
        { selector: "node[type='Enemy']", style: { "background-color": "#d95c55" } },
        { selector: "node[type='Tower']", style: { "background-color": "#2f9d8f" } },
        { selector: ".active", style: { "background-color": "#e7b84c", "line-color": "#e7b84c", width: 4 } }
      ],
      layout: { name: "cose", animate: false }
    });
    cy.on("tap", "node", (event) => {
      const found = nodes.find((node) => node.id === event.target.id());
      if (found) setSelected(found);
    });
    cyInstance.current = cy;
    return () => cy.destroy();
  }, []);

  useEffect(() => {
    const cy = cyInstance.current;
    if (!cy) return;
    cy.elements().removeClass("active");
    active.forEach((step) => cy.getElementById(step.id).addClass("active"));
  }, [active]);

  return (
    <main className="shell">
      <aside className="sidebar">
        <h1>RT Tactical Intelligence</h1>
        {["Dashboard", "Network Graph", "BFS Simulator", "DFS Analyzer", "Threat Center", "Command Center", "Intelligence Search", "Event Monitor", "Connectivity Monitor", "Analytics"].map((item) => (
          <button key={item}>{item}</button>
        ))}
      </aside>
      <section className="workspace">
        <div className="metricGrid">
          {metrics.map(([label, value, Icon]) => (
            <article className="metric" key={String(label)}>
              <Icon size={18} />
              <span>{label}</span>
              <strong>{value}</strong>
            </article>
          ))}
        </div>
        <section className="mainGrid">
          <div className="graphPanel">
            <div className="toolbar">
              <button onClick={() => setActive(bfs("107"))}>Run BFS Threat Spread</button>
              <button onClick={() => setActive(dfs("101"))}>Run DFS Sweep</button>
              <button onClick={() => setActive([{ id: "101", depth: 0 }, { id: "102", depth: 1 }, { id: "108", depth: 2 }])}>Safe Route</button>
            </div>
            <div ref={cyRef} className="cy" />
          </div>
          <aside className="detail">
            <h2>{selected.label}</h2>
            <dl>
              <dt>Type</dt><dd>{selected.type}</dd>
              <dt>Status</dt><dd>{selected.status}</dd>
              <dt>Threat</dt><dd>{selected.threat}/10</dd>
              <dt>Connectivity</dt><dd>{selected.connectivity}%</dd>
            </dl>
            <h3>Traversal Order</h3>
            <ol>{active.map((step) => <li key={`${step.id}-${step.depth}`}>Node {step.id} at level {step.depth}</li>)}</ol>
          </aside>
        </section>
      </section>
    </main>
  );
}
