<script lang="ts">
  import type { PageData } from './$types';
  import ProgramSelect from "./ProgramSelect.svelte"; 

  export let data: PageData;

  export let programs: string[] = [];
</script>


<h1>Welcome to disko</h1>

<ProgramSelect programs={data.programs} />