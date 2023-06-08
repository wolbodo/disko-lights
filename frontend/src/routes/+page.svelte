<script lang="ts">
  import type { PageData } from './$types';
  import ProgramSelect from "./ProgramSelect.svelte"; 
  import { send } from './config'

  let brightness = 125;
  export let data: PageData;

  export let programs: string[] = [];
</script>


<h1>Welcome to disko</h1>

<ProgramSelect programs={data.programs} />


<label>
  Brightnes: 
  <input type='range'  bind:value={brightness} min=1 max=255 on:change={({ target }) => send({ brightness: target.value})}/>
</label>