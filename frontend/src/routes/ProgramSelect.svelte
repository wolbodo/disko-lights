<script lang="ts">
  export let programs: string[] = [];

  const send = (program: string) => {
    fetch("http://disko-grid.local/program", {
      method: "POST",
      headers:{
        'Content-Type': 'application/x-www-form-urlencoded'
      },    
      body: new URLSearchParams({
        program
      })
    })
  }
</script>

<select disabled={!programs.length} on:change={({ target }) => send(target.value) }>
  {#each programs as program}
    <option value={program}>{program}</option>
  {:else}
    <option>Loading</option>
  {/each}
</select>