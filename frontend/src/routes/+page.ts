import type { PageLoad } from './$types';
export const ssr = false;
export const load = (async ({ fetch, params }) => {
  const res = await fetch(`http://disko-grid.local/program`);
  const programs: string[] = await res.json();

  console.log("Done loading")
 
  return { programs };
}) satisfies PageLoad;